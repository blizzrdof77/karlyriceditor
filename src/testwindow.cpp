/***************************************************************************
 *   Copyright (C) 2009 Georgy Yunaev, gyunaev@ulduzsoft.com               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QTextDocument>

#include "testwindow.h"
#include "settings.h"

// How long (ms) to show the line/block before the song actually starts
static const int LYRICS_SHOW_BEFORE = 5000;

// How long (ms) to keep the line/block on screen before hiding it
static const int LYRICS_SHOW_AFTER = 5000;


TestWindow::TestWindow( QWidget *parent )
	: QDialog(parent), Ui::TestWindow()
{
	setupUi( this );
}

void TestWindow::setLyrics( const Lyrics& lyrics )
{
	// Background color
	QPalette pal = palette();
	pal.setColor( QPalette::Window, pSettings->m_previewBackground );
	setPalette( pal );

	// Font
	QFont font( pSettings->m_previewFontFamily, pSettings->m_previewFontSize );
	setFont( font );

	// Clear label
	setText( "" );

	// Index the lyrics
	m_lyricIndex.clear();
	int lineidx = 1;

	// Fill the m_lyricIndex array
	for ( int bl = 0; bl < lyrics.totalBlocks(); bl++ )
	{
		const Lyrics::Block& block = lyrics.block( bl );

		for ( int ln = 0; ln < block.size(); ln++ )
		{
			const Lyrics::Line& line = block[ln];

			for ( int pos = 0; pos < line.size(); pos++ )
			{
				Lyrics::Syllable lentry = line[pos];
				LyricIndex lidx;

				lidx.timestart = lentry.timing;
				lidx.blockindex = bl;
				lidx.lineindex = lineidx;
				lidx.text = lentry.text;

				if ( pos == line.size() - 1 )
				{
					lidx.text += "\n";
					m_lyricIndex.push_back( lidx );
					lineidx++;
				}
				else
					m_lyricIndex.push_back( lidx );
			}
		}
	}

	// Now rescan the index, and split any which have more than one char
	for ( int i = 0; i < m_lyricIndex.size(); i++ )
	{
		if ( m_lyricIndex[i].text.trimmed().length() > 1 )
			splitSyllable( i );
	}

	// Create the m_blockIndex
	int current_block = -1;

	for ( int i = 0; i < m_lyricIndex.size(); i++ )
	{
		if ( m_lyricIndex[i].blockindex != current_block )
		{
			Time blocktime;
			blocktime.timestart = blocktime.timeend = m_lyricIndex[i].timestart;
			blocktime.index = i;
			m_blockIndex.push_back( blocktime );

			current_block = m_lyricIndex[i].blockindex;
		}
		else
			m_blockIndex.back().timeend = m_lyricIndex[i].timestart;
	}

	// Apply before/after delays
	for ( int i = 0; i < m_blockIndex.size(); i++ )
	{
		if ( i == 0 || m_blockIndex[i].timestart - LYRICS_SHOW_BEFORE > m_blockIndex[i-1].timeend )
			m_blockIndex[i].timestart = qMax( (qint64) 0, m_blockIndex[i].timestart - LYRICS_SHOW_BEFORE );
		else
			m_blockIndex[i].timestart = m_blockIndex[i-1].timeend + 1;

		if ( i == m_blockIndex.size() - 1 || m_blockIndex[i].timeend + LYRICS_SHOW_AFTER < m_blockIndex[i+1].timestart )
			m_blockIndex[i].timeend += LYRICS_SHOW_AFTER;
		else
			m_blockIndex[i].timeend = m_blockIndex[i+1].timestart - 1;
	}

	// Dump block index
//	for ( int i = 0; i < m_blockIndex.size(); i++ )
//		qDebug("BlockIndex for block %d: idx %d, %d - %d", i, m_blockIndex[i].index, (int) m_blockIndex[i].timestart, (int) m_blockIndex[i].timeend );

	// Dump regular index
//	for ( int i = 0; i < m_lyricIndex.size(); i++ )
//		qDebug("lyric %d: time %d, block %d, text %s", i, (int) m_lyricIndex[i].timestart, m_lyricIndex[i].blockindex, qPrintable( m_lyricIndex[i].text ) );
}

void TestWindow::tick( qint64 tickmark )
{
	if ( pSettings->m_editorSupportBlocks )
		redrawBlocks( tickmark );
	else
		redrawLines( tickmark );
}

void TestWindow::redrawBlocks( qint64 tickmark )
{
	// Find the block containing current timemark
	int blockid = -1;

	for ( int i = 0; i < m_blockIndex.size(); i++ )
	{
		if ( tickmark >= m_blockIndex[i].timestart && tickmark <= m_blockIndex[i].timeend )
		{
			blockid = i;
			break;
		}
	}

	if ( blockid == -1 )
	{
		// Nothing to show
		setText( "" );
		return;
	}

	// Color to text
	QString colorActive = pSettings->m_previewTextActive.name();
	QString colorInactive = pSettings->m_previewTextInactive.name();
	int index = m_blockIndex[blockid].index;

	// Create a multiline string from current block.
	QString text = "<qt>";

	for ( ; index < m_lyricIndex.size() && m_lyricIndex[index].blockindex == blockid; index++ )
	{
		QString lyrictext = Qt::escape( m_lyricIndex[index].text );
		lyrictext.replace( "\n", "<br>" );

		if ( m_lyricIndex[index].timestart < tickmark )
		{
			// This entry hasn't been played yet
			text += QString("<font color=\"%1\">%2</font>") .arg( colorInactive ) .arg( lyrictext );
		}
		else if ( m_lyricIndex[index].timestart >= tickmark )
		{
			// This entry has been played
			text += QString("<font color=\"%1\">%2</font>") .arg( colorActive ) .arg( lyrictext );
		}
	}

	text += "</qt>";
	setText( text );
}

void TestWindow::redrawLines( qint64 tickmark )
{
}

void TestWindow::setText( const QString& text )
{
	if ( text == m_labelText )
		return;

	m_labelText = text;
	label->setText( m_labelText );
}

void TestWindow::splitSyllable( int index )
{
	LyricIndex orig = m_lyricIndex[ index ];

	qint64 timestart = orig.timestart;
	qint64 timeend = (index == m_lyricIndex.size() - 1) ? timestart : m_lyricIndex[ index + 1 ].timestart;
	QString text = orig.text;
	int ticksperchar = (timeend - timestart) / text.length();

	// Remove the old one - will be replaced by one or more
	m_lyricIndex.remove( index, 1 );

	for ( int i = 0; i < text.length(); i++ )
	{
		LyricIndex lidx;

		lidx.timestart = timestart + i * ticksperchar;
		lidx.blockindex = orig.blockindex;
		lidx.lineindex = orig.lineindex;
		lidx.text = QString( text[i] );

		m_lyricIndex.insert( index, 1, lidx );
		index++;
	}
}
