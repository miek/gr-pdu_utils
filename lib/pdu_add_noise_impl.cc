/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_add_noise_impl.h"

namespace gr {
  namespace pdu_utils {

    pdu_add_noise::sptr
    pdu_add_noise::make(float noise_level, float offset, float scale, long seed)
    {
      return gnuradio::get_initial_sptr
        (new pdu_add_noise_impl(noise_level, offset, scale, seed));
    }

    /*
     * The private constructor
     */
    pdu_add_noise_impl::pdu_add_noise_impl(float noise_level, float offset, float scale, long seed)
      : gr::block("pdu_add_noise",
              io_signature::make (0, 0, 0),
              io_signature::make (0, 0, 0)),
        d_noise_level(noise_level),
        d_offset(offset),
        d_scale(scale),
        d_rng(seed)
    {
      message_port_register_in(PMTCONSTSTR__PDU_IN);
      set_msg_handler(PMTCONSTSTR__PDU_IN,
          boost::bind(&pdu_add_noise_impl::handle_msg, this, _1));
      message_port_register_out(PMTCONSTSTR__PDU_OUT);
    }

    /*
     * Our virtual destructor.
     */
    pdu_add_noise_impl::~pdu_add_noise_impl()
    {
    }


    void
    pdu_add_noise_impl::handle_msg(pmt::pmt_t pdu)
    {
      // make sure PDU data is formed properly
      if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
      }

      gr::thread::scoped_lock l(d_setlock);

      /* code */
      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t v_data = pmt::cdr(pdu);

      if (pmt::is_u8vector(v_data)) {
        const std::vector<uint8_t> input = pmt::u8vector_elements(v_data);
        uint32_t v_len = input.size();
        std::vector<uint8_t> out;
        out.resize(v_len);

        for (int ii=0; ii < v_len; ii++) {
          //u8 noise is [0-1]
          out[ii] = uint8_t ((input[ii] + ((d_rng.ran1()) * d_noise_level)) * d_scale + d_offset);
        }
        message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, pmt::init_u8vector(v_len, out))));

      } else if (pmt::is_f32vector(v_data)) {
        const std::vector<float> input = pmt::f32vector_elements(v_data);
        uint32_t v_len = input.size();
        std::vector<float> out;
        out.resize(v_len);

        for (int ii=0; ii < v_len; ii++) {
          out[ii] = (input[ii] + ((d_rng.ran1()*2 - 1) * d_noise_level)) * d_scale + d_offset;
        }
        message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, pmt::init_f32vector(v_len, out))));

      } else if (pmt::is_c32vector(v_data)) {
        const std::vector<gr_complex> input = pmt::c32vector_elements(v_data);
        uint32_t v_len = input.size();
        std::vector<gr_complex> out;
        out.resize(v_len);

        for (int ii=0; ii < v_len; ii++) {
          out[ii] = (input[ii] + ((d_rng.ran1()*2 - 1) * d_noise_level)) * d_scale + d_offset;
        }
        message_port_pub(PMTCONSTSTR__PDU_OUT, (pmt::cons(meta, pmt::init_c32vector(v_len, out))));

      } else {
        GR_LOG_WARN(d_logger, "unsupported PDU type received");
      }
    }

    void
    pdu_add_noise_impl::set_noise_level(float nl)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_noise_level = nl;
    }

    void
    pdu_add_noise_impl::set_offset(float o)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_offset = o;
    }

    void
    pdu_add_noise_impl::set_scale(float s)
    {
      gr::thread::scoped_lock l(d_setlock);

      d_scale = s;
    }

  } /* namespace pdu_utils */
} /* namespace gr */
