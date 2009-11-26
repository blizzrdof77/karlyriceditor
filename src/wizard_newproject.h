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

#ifndef WIZARD_NEWPROJECT_H
#define WIZARD_NEWPROJECT_H

#include <QWizardPage>
#include <phonon/mediaobject.h>

#include "ui_wiznewproject_musicfile.h"
#include "ui_wiznewproject_lyrics.h"
#include "ui_wiznewproject_intro.h"
#include "ui_wiznewproject_finish.h"
#include "ui_wiznewproject_lyrictype.h"

class Project;

namespace WizardNewProject
{

// Intro page
class PageIntro : public QWizardPage, public Ui::WizNewProject_Intro
{
	Q_OBJECT

	public:
		PageIntro( Project *, QWidget *parent = 0 )
			: QWizardPage( parent ), Ui::WizNewProject_Intro()
		{
			setupUi( this );
		}
};


// "Select lyrics type" page
class PageLyricType : public QWizardPage, public Ui::WizNewProject_LyricType
{
	Q_OBJECT

	public:
		PageLyricType( Project * project, QWidget *parent = 0 );

		// overriden
		bool validatePage();

	public slots:
		void	showhelp();

	private:
		Project *	m_project;
};


// "Choose music file" page
class PageMusicFile : public QWizardPage, public Ui::WizNewProject_MusicFile
{
	Q_OBJECT

	public:
		PageMusicFile( Project * project, QWidget *parent = 0 );
		~PageMusicFile();

		// overriden
		bool validatePage();

	public slots:
		void	phonon_StateChanged ( Phonon::State newstate, Phonon::State oldstate );
		void	browse();

	private:
		Phonon::MediaObject *	m_mediaObject;
		Project *	m_project;
};


// "Choose lyrics" page
class PageLyrics : public QWizardPage, public Ui::WizNewProject_Lyrics
{
	Q_OBJECT

	public:
		PageLyrics( Project * project, QWidget *parent = 0 );

		// overriden
		void initializePage();
		bool validatePage();

	private:
		Project *	m_project;
};

// "Finish" page
class PageFinish : public QWizardPage, public Ui::WizNewProject_Finish
{
	Q_OBJECT

	public:
		PageFinish( Project *, QWidget *parent = 0 )
			: QWizardPage( parent ), Ui::WizNewProject_Finish()
		{
			setupUi( this );
		}
};


// New project wizard
class Wizard : public QWizard
{
	Q_OBJECT

	public:
		Wizard( Project * project, QWidget *parent = 0 );
};

} // namespace WizardNewProject


#endif // WIZARD_NEWPROJECT_H
