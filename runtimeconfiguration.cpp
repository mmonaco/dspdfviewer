/*
    dspdfviewer - Dual Screen PDF Viewer for LaTeX-Beamer
    Copyright (C) 2012  Danny Edel <mail@danny-edel.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "runtimeconfiguration.h"

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

#ifndef DSPDFVIEWER_VERSION
#warning DSPDFVIEWER_VERSION was not set by the build system!
#define DSPDFVIEWER_VERSION "UNKNOWN"
#endif

static const char* OPTSTRING = "hf1P:N:atwps";
static const struct option LONG_OPTS[] = {
	{ "help",                   no_argument,       NULL, 'h' },
	{ "usage",                  no_argument,       NULL, 'u' },
	{ "full-page",              no_argument,       NULL, 'f' },
	{ "single-screen",          no_argument,       NULL, '1' },
	{ "previous",               required_argument, NULL, 'P' },
	{ "next",                   required_argument, NULL, 'N' },
	{ "no-presenter-area",      no_argument,       NULL, 'a' },
	{ "no-thumbnails",          no_argument,       NULL, 't' },
	{ "no-wall-clock",          no_argument,       NULL, 'w' },
	{ "no-presentation-clock",  no_argument,       NULL, 'p' },
	{ "no-slide-clock",         no_argument,       NULL, 's' },
	{ NULL, 0, NULL, 0 }
};

static void usage(const char* argv0)
{
	printf(
	"usage: %s [OPTIONS] PDF\n"
	"\n"
	"  This program displays PDFs generated using beamer's \"show notes\n"
	"  on second screen\" option. The primary monitor displays presenter\n"
	"  notes, slide previews, and timers. The secondary monitor displays\n"
	"  the presentation itself for the audience.\n"
	"\n"
	"  general options:\n"
	"    -f --full-page               display a standard PDF on both monitors\n"
	"    -1 --single-screen           only use a single screen\n"
	"    -h --help                    this message\n"
	"\n"
	"  presenter options:\n"
	"    -P --previous N              render N previous frames, default 3, 0 disables\n"
	"    -N --next N                  render N upcoming frames, default 3, 0 disables\n"
	"    -a --no-presenter-area       only render a full-screen note page on the presenter screen\n"
	"    -t --no-thumnails            do not render thumbnails of previous, current, and next frame\n"
	"    -w --no-wall-clock           disable the wall clock\n"
	"    -p --no-presentation-clock   disable the presentation clock\n"
	"    -s --no-slide-clock          disable the slide clock\n"
	"\n"
	"Version " DSPDFVIEWER_VERSION "\n"
	"Copyright (C) 2013 Danny Edel\n"
	"\n",
		argv0
	);
}

RuntimeConfiguration::RuntimeConfiguration(int argc, char* argv[])
{
	int c, opt_index;

	this->m_useFullPage = false;
	this->m_useSecondScreen = true;
	this->m_prerenderPreviousPages = 3;
	this->m_prerenderNextPages = 3;
	this->m_showPresenterArea = true;
	this->m_showThumbnails = true;
	this->m_showWallClock = true;
	this->m_showPresentationClock = true;
	this->m_showSlideClock = true;

	while ((c = getopt_long(argc, argv, OPTSTRING, LONG_OPTS, &opt_index)) != -1) {

		switch(c) {
		case 'h':
			usage(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		case 'f':
			this->m_useFullPage = true;
			break;
		case '1':
			this->m_useSecondScreen = false;
			break;
		case 'P':
			this->m_prerenderPreviousPages = strtoul(optarg, NULL, 0);
			break;
		case 'N':
			this->m_prerenderNextPages = strtoul(optarg, NULL, 0);
			break;
		case 'a':
			this->m_showPresenterArea = false;
			break;
		case 't':
			this->m_showThumbnails = false;
			break;
		case 'w':
			this->m_showWallClock = false;
			break;
		case 'p':
			this->m_showPresentationClock = false;
			break;
		case 's':
			this->m_showSlideClock = false;
			break;
		default:
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 1) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	this->m_filePath = argv[optind];

	if (!this->m_useSecondScreen)
		this->m_showPresenterArea = false;

	if (!this->m_showPresenterArea) {
		this->m_showThumbnails = false;
		this->m_showWallClock = false;
		this->m_showPresentationClock = false;
		this->m_showSlideClock = false;
	}
}

std::string RuntimeConfiguration::filePath() const
{
  return m_filePath;
}

QString RuntimeConfiguration::filePathQString() const
{
  return QString::fromStdString(m_filePath);
}

bool RuntimeConfiguration::useFullPage() const
{
  return m_useFullPage;
}

bool RuntimeConfiguration::showPresentationClock() const
{
  return m_showPresentationClock;
}
bool RuntimeConfiguration::showPresenterArea() const
{
  return m_showPresenterArea;
}
bool RuntimeConfiguration::showSlideClock() const
{
  return m_showSlideClock;
}
bool RuntimeConfiguration::showThumbnails() const
{
  return m_showThumbnails;
}
bool RuntimeConfiguration::showWallClock() const
{
  return m_showWallClock;
}

unsigned int RuntimeConfiguration::prerenderNextPages() const
{
  return m_prerenderNextPages;
}
unsigned int RuntimeConfiguration::prerenderPreviousPages() const
{
  return m_prerenderPreviousPages;
}

bool RuntimeConfiguration::useSecondScreen() const
{
  return m_useSecondScreen;
}

