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


#ifndef INCLUDED_PDU_UTILS_PDU_ALIGN_H
#define INCLUDED_PDU_UTILS_PDU_ALIGN_H

#include <pdu_utils/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace pdu_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup pdu_utils
     *
     */
    class PDU_UTILS_API pdu_align : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<pdu_align> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_align.
       *
       * To avoid accidental use of raw pointers, pdu_utils::pdu_align's
       * constructor is in a private implementation
       * class. pdu_utils::pdu_align::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string syncwords, int threshold, int offset);
    };

  } // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_ALIGN_H */

