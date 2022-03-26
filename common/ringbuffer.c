/**
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "ringbuffer.h"

#include <stdlib.h>
#include <string.h>

#define WR_OFFSET 0
#define RD_OFFSET 1

void ringbuf_init(ringbuf_t const* p_ringbuf)
{
    p_ringbuf->p_cb->wr_idx     = 0;
    p_ringbuf->p_cb->rd_idx     = 0;
    p_ringbuf->p_cb->tmp_rd_idx = 0;
    p_ringbuf->p_cb->tmp_wr_idx = 0;
    p_ringbuf->p_cb->rd_flag    = 0;
    p_ringbuf->p_cb->wr_flag    = 0;
}

int ringbuf_alloc(ringbuf_t const* p_ringbuf, uint8_t** pp_data, size_t* p_length, bool start)
{
    assert(pp_data);
    assert(p_length);

    if(start) {
        if(__atomic_fetch_or(&p_ringbuf->p_cb->wr_flag, 1, __ATOMIC_SEQ_CST)) {
            return -1;
        }
    }

    if(p_ringbuf->p_cb->tmp_wr_idx - p_ringbuf->p_cb->rd_idx == p_ringbuf->bufsize_mask + 1) {
        *p_length = 0;
        if(start) {
            __atomic_and_fetch(&p_ringbuf->p_cb->wr_flag, 0, __ATOMIC_SEQ_CST);
        }
        return 0;
    }

    uint32_t wr_idx    = p_ringbuf->p_cb->tmp_wr_idx & p_ringbuf->bufsize_mask;
    uint32_t rd_idx    = p_ringbuf->p_cb->rd_idx & p_ringbuf->bufsize_mask;
    uint32_t available = (wr_idx >= rd_idx) ? p_ringbuf->bufsize_mask + 1 - wr_idx : p_ringbuf->p_cb->rd_idx - (p_ringbuf->p_cb->tmp_wr_idx - (p_ringbuf->bufsize_mask + 1));
    *p_length          = *p_length < available ? *p_length : available;
    *pp_data           = &p_ringbuf->p_buffer[wr_idx];
    p_ringbuf->p_cb->tmp_wr_idx += *p_length;

    return 0;
}

int ringbuf_put(ringbuf_t const* p_ringbuf, size_t length)
{
    uint32_t available = p_ringbuf->bufsize_mask + 1 - (p_ringbuf->p_cb->wr_idx - p_ringbuf->p_cb->rd_idx);
    if(length > available) {
        return -1;
    }

    p_ringbuf->p_cb->wr_idx += length;
    p_ringbuf->p_cb->tmp_wr_idx = p_ringbuf->p_cb->wr_idx;
    if(__atomic_fetch_and(&p_ringbuf->p_cb->wr_flag, 0, __ATOMIC_SEQ_CST) == 0) {
        /* Flag was already cleared. Suggests misuse. */
        return -1;
    }
    return 0;
}

int ringbuf_cpy_put(ringbuf_t const* p_ringbuf, uint8_t const* p_data, size_t* p_length)
{
    assert(p_data);
    assert(p_length);

    if(__atomic_fetch_or(&p_ringbuf->p_cb->wr_flag, 1, __ATOMIC_SEQ_CST)) {
        return -1;
    }

    uint32_t available     = p_ringbuf->bufsize_mask + 1 - (p_ringbuf->p_cb->wr_idx - p_ringbuf->p_cb->rd_idx);
    *p_length              = available > *p_length ? *p_length : available;
    size_t   length        = *p_length;
    uint32_t masked_wr_idx = (p_ringbuf->p_cb->wr_idx & p_ringbuf->bufsize_mask);
    uint32_t trail         = p_ringbuf->bufsize_mask + 1 - masked_wr_idx;

    if(length > trail) {
        memcpy(&p_ringbuf->p_buffer[masked_wr_idx], p_data, trail);
        length -= trail;
        masked_wr_idx = 0;
        p_data += trail;
    }
    memcpy(&p_ringbuf->p_buffer[masked_wr_idx], p_data, length);
    p_ringbuf->p_cb->wr_idx += *p_length;
    p_ringbuf->p_cb->tmp_wr_idx = p_ringbuf->p_cb->wr_idx;

    __atomic_and_fetch(&p_ringbuf->p_cb->wr_flag, 0, __ATOMIC_SEQ_CST);

    return 0;
}

int ringbuf_get(ringbuf_t const* p_ringbuf, uint8_t** pp_data, size_t* p_length, bool start)
{
    assert(pp_data);
    assert(p_length);

    if(start) {
        if(__atomic_fetch_or(&p_ringbuf->p_cb->rd_flag, 1, __ATOMIC_SEQ_CST)) {
            return -1;
        }
    }

    uint32_t available = p_ringbuf->p_cb->wr_idx - p_ringbuf->p_cb->tmp_rd_idx;
    if(available == 0) {
        *p_length = 0;
        if(start) {
            __atomic_and_fetch(&p_ringbuf->p_cb->rd_flag, 0, __ATOMIC_SEQ_CST);
        }
        return 0;
    }

    uint32_t masked_tmp_rd_idx = p_ringbuf->p_cb->tmp_rd_idx & p_ringbuf->bufsize_mask;
    uint32_t masked_wr_idx     = p_ringbuf->p_cb->wr_idx & p_ringbuf->bufsize_mask;

    if((masked_wr_idx > masked_tmp_rd_idx) && (available < *p_length)) {
        *p_length = available;
    }
    else if(masked_wr_idx <= masked_tmp_rd_idx) {
        uint32_t trail = p_ringbuf->bufsize_mask + 1 - masked_tmp_rd_idx;
        if(*p_length > trail) {
            *p_length = trail;
        }
    }
    *pp_data = &p_ringbuf->p_buffer[masked_tmp_rd_idx];
    p_ringbuf->p_cb->tmp_rd_idx += *p_length;

    return 0;
}

int ringbuf_cpy_get(ringbuf_t const* p_ringbuf, uint8_t* p_data, size_t* p_length)
{
    assert(p_data);
    assert(p_length);

    if(__atomic_fetch_or(&p_ringbuf->p_cb->rd_flag, 1, __ATOMIC_SEQ_CST)) {
        return -1;
    }

    uint32_t available     = p_ringbuf->p_cb->wr_idx - p_ringbuf->p_cb->rd_idx;
    *p_length              = available > *p_length ? *p_length : available;
    size_t   length        = *p_length;
    uint32_t masked_rd_idx = (p_ringbuf->p_cb->rd_idx & p_ringbuf->bufsize_mask);
    uint32_t masked_wr_idx = (p_ringbuf->p_cb->wr_idx & p_ringbuf->bufsize_mask);
    uint32_t trail         = (masked_wr_idx > masked_rd_idx) ? masked_wr_idx - masked_rd_idx : p_ringbuf->bufsize_mask + 1 - masked_rd_idx;

    if(length > trail) {
        memcpy(p_data, &p_ringbuf->p_buffer[masked_rd_idx], trail);
        length -= trail;
        masked_rd_idx = 0;
        p_data += trail;
    }
    memcpy(p_data, &p_ringbuf->p_buffer[masked_rd_idx], length);
    p_ringbuf->p_cb->rd_idx += *p_length;
    p_ringbuf->p_cb->tmp_rd_idx = p_ringbuf->p_cb->rd_idx;

    __atomic_and_fetch(&p_ringbuf->p_cb->rd_flag, 0, __ATOMIC_SEQ_CST);

    return 0;
}

int ringbuf_free(ringbuf_t const* p_ringbuf, size_t length)
{
    uint32_t available = (p_ringbuf->p_cb->wr_idx - p_ringbuf->p_cb->rd_idx);
    if(length > available) {
        return -1;
    }

    p_ringbuf->p_cb->rd_idx += length;
    p_ringbuf->p_cb->tmp_rd_idx = p_ringbuf->p_cb->rd_idx;
    __atomic_and_fetch(&p_ringbuf->p_cb->rd_flag, 0, __ATOMIC_SEQ_CST);
    return 0;
}
