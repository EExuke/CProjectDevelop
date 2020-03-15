/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : Mp3.h
 *     FIRST CREATION DATE    : 2020/03/15
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : xuke
 *     Last Change            : 2020/03/15
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <gst/gst.h>
static GstElement *play = NULL;
static guint timeout_source = 0;
static GtkWidget *main_window;
static GtkWidget *play_button;
static GtkWidget *pause_button;
static GtkWidget *stop_button;
static GtkWidget *open_file;
static GtkWidget *status_label;
static GtkWidget *time_label;
static GtkWidget *seek_scale;
static GtkWidget *title_label;
static GtkWidget *artist_label;
static char *current_filename = NULL;
gboolean no_seek = FALSE;

static void open_file_clicked(GtkWidget *widget, gpointer data);
static void pause_clicked(GtkWidget *widget, gpointer data);
gboolean pause_play();
static gboolean build_gstreamer_pipeline(const gchar *uri);
static gboolean bus_callback (GstBus *bus, GstMessage *message, gpointer data);
void gui_clear_metadata(void);
void gui_update_time(const gchar *time,
                     const gint64 position,const gint64 length);
void gui_update_metadata(const gchar *title,const gchar *artist);
static void open_file_clicked(GtkWidget *widget, gpointer data);
gboolean load_file(const gchar *uri);
gboolean play_file();
void seek_to(gdouble percentage);
static void play_clicked(GtkWidget *widget, gpointer data);
gboolean play_file();
static void seek_value_changed(GtkRange *range, gpointer data);
static void stop_clicked(GtkWidget *widget, gpointer data);
void initgui();
#endif
