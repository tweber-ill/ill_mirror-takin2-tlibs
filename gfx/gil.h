/**
 * gil wrappers
 * @author Tobias Weber <tobias.weber@tum.de>
 * @date dec-2015
 * @license GPLv2 or GPLv3
 *
 * ----------------------------------------------------------------------------
 * tlibs -- a physical-mathematical C++ template library
 * Copyright (C) 2017-2021  Tobias WEBER (Institut Laue-Langevin (ILL),
 *                          Grenoble, France).
 * Copyright (C) 2015-2017  Tobias WEBER (Technische Universitaet Muenchen
 *                          (TUM), Garching, Germany).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 */

#ifndef __TLIBS_GIL_H__
#define __TLIBS_GIL_H__

#include <vector>
#include <memory>
#include <functional>

#include "../log/log.h"
#include "../string/string.h"
#include "../helper/exception.h"

#ifndef int_p_NULL
	#define int_p_NULL reinterpret_cast<int*>(0)
#endif
#include <boost/gil.hpp>

#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/io/png/old.hpp>

#ifndef NO_JPEG
	#include <boost/gil/extension/io/jpeg.hpp>
	#include <boost/gil/extension/io/jpeg/old.hpp>
#endif

#ifndef NO_TIFF
	#include <boost/gil/extension/io/tiff.hpp>
	#include <boost/gil/extension/io/tiff/old.hpp>
#endif


namespace tl
{
	namespace gil = boost::gil;

	template<class t_pixel=gil::rgb8_image_t, bool bPlanar=true, class t_char=char>
	std::shared_ptr<gil::image<t_pixel, bPlanar>> load_image(const t_char* pcFile)
	{
		using t_img = gil::image<t_pixel, bPlanar>;
		using t_str = std::basic_string<t_char>;

		std::shared_ptr<t_img> pimg = std::make_shared<t_img>();
		t_str strExt = tl::str_to_lower<t_str>(get_fileext<t_str>(pcFile));

		try
		{
			if(strExt == "png")
				gil::png_read_image(pcFile, *pimg);
#ifndef NO_TIFF
			else if(strExt == "tif" || strExt == "tiff")
				gil::tiff_read_image(pcFile, *pimg);
#endif
#ifndef NO_JPEG
			else if(strExt == "jpg" || strExt == "jpeg")
				gil::jpeg_read_image(pcFile, *pimg);
#endif
			else
				throw Err("Unknown file extension in image loading.");
		}
		catch(const std::exception& ex)
		{
			tl::log_err(ex.what());
			return std::shared_ptr<t_img>(nullptr);
		}
		return pimg;
	}


	template<class t_viewptr, class t_char=char>
	bool save_view(const t_char* pcFile, const t_viewptr pview)
	{
		using t_str = std::basic_string<t_char>;
		t_str strExt = tl::str_to_lower<t_str>(get_fileext<t_str>(pcFile));

		try
		{
			if(strExt == "png")
				gil::png_write_view(pcFile, *pview);
#ifndef NO_TIFF
			else if(strExt == "tif" || strExt == "tiff")
				gil::tiff_write_view(pcFile, *pview);
#endif
#ifndef NO_JPEG
			else if(strExt == "jpg" || strExt == "jpeg")
				gil::jpeg_write_view(pcFile, *pview, 90);
#endif
			else
				throw Err("Unknown file extension in image saving.");
		}
		catch(const std::exception& ex)
		{
			tl::log_err(ex.what());
			return 0;
		}
		return 1;
	}

	template<class t_imgptr, class t_char=char>
	bool save_image(const t_char* pcFile, const t_imgptr pimg)
	{
		return save_view(pcFile, &gil::view(*pimg));
	}


	template<class t_view = gil::rgb8_view_t,
		class t_pix = typename t_view::value_type>
	inline void create_imgview(std::size_t iW, std::size_t iH,
		std::vector<t_pix>& vecPix, t_view& view)
	{
		vecPix.resize(iW*iH);
		view = gil::interleaved_view(iW, iH, vecPix.data(), iW*sizeof(t_pix));
	}


	template<class t_view = gil::rgb8_view_t,
		class t_pix = typename t_view::value_type,
		class t_func = std::function<void(t_pix&)>>
	void for_each_in_view(t_view& view, t_func fkt)
	{
		for(std::size_t iY=0; iY<view.height(); ++iY)
			for(auto iterX=view.row_begin(iY); iterX!=view.row_end(iY); ++iterX)
			{
				t_pix pix = *iterX;
				fkt(pix);
				*iterX = pix;
			}
	}


	// gets average background counts in roi
	template<class t_view = gil::rgb8_view_t,
		class t_pix = typename t_view::value_type>
	unsigned long get_roi_avg(t_view& view,
		std::size_t iXStart, std::size_t iXEnd,
		std::size_t iYStart, std::size_t iYEnd)
	{
		unsigned long iCnts = 0;
		unsigned long iPix = 0;

		for(std::size_t iY=iYStart; iY<iYEnd; ++iY)
			for(auto iterX=view.row_begin(iY)+iXStart; iterX!=view.row_begin(iY)+iXEnd; ++iterX)
			{
				++iPix;
				iCnts += *iterX;
			}

		return (unsigned long)(double(iCnts)/double(iPix));
	}

	// integrates counts in roi
	template<class t_view = gil::rgb8_view_t,
		class t_pix = typename t_view::value_type>
	unsigned long get_roi_sum(t_view& view,
		std::size_t iXStart, std::size_t iXEnd,
		std::size_t iYStart, std::size_t iYEnd,
		unsigned long iBkg=0)
	{
		unsigned long iCnts = 0;

		for(std::size_t iY=iYStart; iY<iYEnd; ++iY)
			for(auto iterX=view.row_begin(iY)+iXStart; iterX!=view.row_begin(iY)+iXEnd; ++iterX)
			{
				if(*iterX > iBkg)
					iCnts += (*iterX - iBkg);
			}

		return iCnts;
	}


	template<class t_view = gil::rgb8_view_t,
		template<class...> class t_cont = std::vector>
	t_cont<typename t_view::value_type> view_to_container(const t_view& view)
	{
		using t_pix = typename t_view::value_type;

		const std::size_t iW = view.width();
		const std::size_t iH = view.height();

		t_cont<t_pix> vecRet;
		vecRet.reserve(iW*iH);

		for(std::size_t iY=0; iY<iH; ++iY)
			for(auto iterX=view.row_begin(iY); iterX!=view.row_end(iY); ++iterX)
				vecRet.push_back(*iterX);

		return vecRet;
	}
}

#endif
