/*********************
Name: Jared Wasinger
ONID Email: wasingej@onid.oregonstate.edu
Class: CS 344
Assignment #2
References:  
**********************/

#ifndef TOC_H
#define TOC_H

#include <grp.h>
#include <pwd.h>
#include <time.h>

#include "oscar.h"
#include "archive.h"

void disp_archive_toc(struct Archive *archive);

void disp_archive_long_toc(struct Archive *archive);

#endif
