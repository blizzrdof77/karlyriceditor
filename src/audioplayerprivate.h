#ifndef AUDIOPLAYERPRIVATE_H
#define AUDIOPLAYERPRIVATE_H

#include <QMutex>
#include <QString>

#include "ffmpeg_headers.h"
#include <SDL/SDL.h>


class AudioPlayerPrivate
{
	public:
		AudioPlayerPrivate();

		bool	init();
		bool	open( const QString& filename );
		void	close();
		void	play();
		void	reset();
		void	stop();
		void	seekTo( qint64 value );
		bool	isPlaying() const;
		qint64	currentTime() const;
		qint64	totalTime() const;
		QString	errorMsg() const;

		// Called from SDL
		void	SDL_audio_callback( Uint8 *stream, int len);

	private:
		bool	MoreAudio();
		void	queueClear();

	private:
		QString			m_errorMsg;

		// Access to everything below is guarded by mutex
		mutable QMutex	m_mutex;

		AVFormatContext *pFormatCtx;
		int				 audioStream;
		AVCodecContext  *aCodecCtx;
		AVCodec         *pCodec;

		bool			m_playing;
		qint64			m_currentTime;
		qint64			m_totalTime;

		// Currently processed frame
		QByteArray		m_sample_buffer;
		unsigned int	m_sample_buf_size;
		unsigned int	m_sample_buf_idx;
};

#endif // AUDIOPLAYERPRIVATE_H
