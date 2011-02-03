/**************************************************************************
 *  Karlyriceditor - a lyrics editor for Karaoke songs                    *
 *  Copyright (C) 2009 George Yunaev, support@karlyriceditor.com          *
 *                                                                        *
 *  This program is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *																	      *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 **************************************************************************/

#ifndef FFMPEGVIDEODECODER_H
#define FFMPEGVIDEODECODER_H

#include <QImage>

class FFMpegVideoDecoderPriv;

class FFMpegVideoDecoder
{
	public:
		FFMpegVideoDecoder();
		~FFMpegVideoDecoder();

		bool openFile( const QString& file );
		QString errorMsg() const;
		void close();

		// Player function
		QImage frame( qint64 timems );

	protected:
		FFMpegVideoDecoderPriv * d;
};

#endif // FFMPEGVIDEODECODER_H
