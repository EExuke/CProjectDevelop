/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : Mp3.c
 *     FIRST CREATION DATE    : 2020/03/15
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : xuke
 *     Last Change            : 2020/03/15
 *     FILE DESCRIPTION       :
** ************************************************************************** */
#include "../inc/Mp3.h"

int main(int argc, char *argv[])
{
	GtkBuilder *builder;
	gtk_init(&argc, &argv);
	gst_init(&argc, &argv);
	builder= gtk_builder_new();
	gtk_builder_add_from_file(builder, "Mp3.glade", NULL);
	main_window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
	//gtk_widget_set_size_request(main_window, 300, 260);
	play_button = GTK_WIDGET(gtk_builder_get_object(builder, "play_button"));
	pause_button = GTK_WIDGET(gtk_builder_get_object(builder, "pause_button"));
	stop_button = GTK_WIDGET(gtk_builder_get_object(builder, "stop_button"));
	open_file = GTK_WIDGET(gtk_builder_get_object(builder, "open_file"));
	status_label = GTK_WIDGET(gtk_builder_get_object(builder, "status_label"));
	time_label = GTK_WIDGET(gtk_builder_get_object(builder, "time_label"));
	seek_scale = GTK_WIDGET(gtk_builder_get_object(builder, "seek_scale"));
	//GtkAdjustment  *adj=gtk_adjustment_new(0,0,100,1,1,100);
	gtk_range_set_adjustment(GTK_SCALE(seek_scale),
			GTK_ADJUSTMENT(gtk_adjustment_new(0,0,100,1,1,0.1)));

	artist_label = GTK_WIDGET(gtk_builder_get_object(builder, "artist_label"));
	title_label = GTK_WIDGET(gtk_builder_get_object(builder, "title_label"));
    gtk_widget_set_sensitive(GTK_WIDGET(stop_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(play_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(pause_button), FALSE);

	g_signal_connect(play_button, "clicked", G_CALLBACK(play_clicked), NULL);
	g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_clicked), NULL);
	g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_clicked), NULL);
	g_signal_connect(seek_scale, "value-changed", G_CALLBACK(seek_value_changed), NULL);
	g_signal_connect(open_file, "clicked", G_CALLBACK(open_file_clicked), NULL);


	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show_all(main_window);
	gtk_main();
	return 0;
}

/* Handler for File->Open action */
static void open_file_clicked(GtkWidget *widget, gpointer data)
{
  /* Construct a GtkFileChooser */
  GtkWidget *file_chooser = gtk_file_chooser_dialog_new(
      "Open File", GTK_WINDOW(main_window),
      GTK_FILE_CHOOSER_ACTION_OPEN,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
      NULL);

  /* Run the dialog and if the user pressed the accept button... */
  if (gtk_dialog_run(GTK_DIALOG(file_chooser)) == GTK_RESPONSE_ACCEPT)
  {
    /* ...get the URI of the chosen file */
    char *filename;
    filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(file_chooser));
    /* In case we're already playing a file, simulate a press of the stop button */
    g_signal_emit_by_name(G_OBJECT(stop_button), "clicked");
    if (current_filename) g_free(current_filename);
    current_filename = filename;
    /* Load the selected file */
    if (load_file(filename))
      gtk_widget_set_sensitive(GTK_WIDGET(play_button), TRUE);
  }

  gtk_widget_destroy(file_chooser);
}

/* Handler for play button click event */
static void play_clicked(GtkWidget *widget, gpointer data)
{
  if (current_filename)
  {
    if (play_file())
    {
      gtk_widget_set_sensitive(GTK_WIDGET(stop_button), TRUE);
      gtk_widget_set_sensitive(GTK_WIDGET(pause_button), TRUE);
    }
    else
    {
      g_print("Failed to play\n");
    }
  }
}
static void pause_clicked(GtkWidget *widget, gpointer data)
{
	  if (play) {
			GstState state;
			gst_element_get_state(play, &state, NULL, -1);
			if(state == GST_STATE_PLAYING){
				gst_element_set_state(play, GST_STATE_PAUSED);
				gtk_button_set_label(GTK_BUTTON(pause_button), "继续");
	           gtk_widget_set_sensitive(GTK_WIDGET(stop_button), FALSE);
	           gtk_widget_set_sensitive(GTK_WIDGET(play_button), FALSE);
			}
			else if(state == GST_STATE_PAUSED){
				gst_element_set_state(play, GST_STATE_PLAYING);
		       gtk_button_set_label(GTK_BUTTON(pause_button), "暂停");
	          gtk_widget_set_sensitive(GTK_WIDGET(stop_button), TRUE);
	          gtk_widget_set_sensitive(GTK_WIDGET(play_button), TRUE);
			}
			return ;
	  }
}
/* Handler for stop button click */
static void stop_clicked(GtkWidget *widget, gpointer data)
{
	  /* Remove the timeout function */
	  if (timeout_source) g_source_remove(timeout_source);
	  timeout_source = 0;

	  /* Stop playback and delete the pipeline */
	  if (play) {
	    gst_element_set_state(play, GST_STATE_NULL);
	    //gst_object_unref(GST_OBJECT(play));
	    //play = NULL;
	  }

	  /* Update the GUI */
	  initgui();
}

/* Handler for user moving seek bar */
static void seek_value_changed(GtkRange *range, gpointer data)
{
  if (no_seek) return;
  gdouble val = gtk_range_get_value(range);

  seek_to(val);
}
/* Takes time values and formats them for the time label and seek slider */
void gui_update_time(const gchar *time, const gint64 position, const gint64 length)
{
  gtk_label_set_text(GTK_LABEL(time_label), time);
  if (length > 0) {
    no_seek = TRUE;
    gtk_range_set_value(GTK_RANGE(seek_scale), ((gdouble)position / (gdouble)length) * 100.0);
    no_seek = FALSE;
  }
}

/* Convenience function to update title and artist display */
void gui_update_metadata(const gchar *title, const gchar *artist)
{
  gtk_label_set_text(GTK_LABEL(title_label), title);
  gtk_label_set_text(GTK_LABEL(artist_label), artist);
}

/* Convenience function to stop displaying metadata */
void gui_clear_metadata(void)
{
  gtk_label_set_text(GTK_LABEL(title_label), "");
  gtk_label_set_text(GTK_LABEL(artist_label), "");
}


/* Callback function invoked when a message arrives on the playback
 * pipeline's bus. */
static gboolean bus_callback (GstBus *bus, GstMessage *message, gpointer data)
{
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
      /* An error has occurred.
       * A real application would probably need to handle this more
       * intelligently than just quitting. */
      GError *err;
      gchar *debug;

      gst_message_parse_error(message, &err, &debug);
      g_print("Error: %s\n", err->message);
      g_error_free(err);
      g_free(debug);

      gtk_main_quit();
      break;
    }

    case GST_MESSAGE_EOS:
      /* The pipeline has reached the end of the stream. */
      g_print("End of stream\n");
      g_signal_emit_by_name(G_OBJECT(stop_button), "clicked");
      initgui();
      break;

    case GST_MESSAGE_TAG: {
      /* The stream discovered new tags. */
      GstTagList *tags;
      gchar *title  = "";
      gchar *artist = "";
      /* Extract from the message the GstTagList.
       * This generates a copy, so we must remember to free it.*/
      gst_message_parse_tag(message, &tags);
      /* Extract the title and artist tags - if they exist */
      if (gst_tag_list_get_string(tags, GST_TAG_TITLE, &title)
	  && gst_tag_list_get_string(tags, GST_TAG_ARTIST, &artist))
	gui_update_metadata(title, artist);
      /* Free the tag list */
      gst_tag_list_free(tags);
      break;
    }

    default:
      /* Another message occurred which we are not interested in handling. */
      break;
  }

  /* We have handled this message, so indicate that it should be removed from
   * the queue.*/
  return TRUE;
}


/* This function is called every 200 milliseconds.
 * It retrieves the pipeline's playback position and updates the GUI with it. */
static gboolean update_time_callback(GstElement *pipeline)
{
  GstFormat fmt = GST_FORMAT_TIME;
  gint64 position;
  gint64 length;
  gchar time_buffer[25];

  if (gst_element_query_position(pipeline, &fmt, &position) && gst_element_query_duration(pipeline, &fmt, &length)) {
    g_snprintf(time_buffer, 24, "%u:%02u:%02u", GST_TIME_ARGS(position));
    gui_update_time(time_buffer, position, length);
  }

  return TRUE;
}


/* Given a URI, constructs a pipeline to play it.
 * Uses GStreamer's supplied 'playbin' element as an automatic solution */
static gboolean build_gstreamer_pipeline(const gchar *uri)
{
  /* Destroy the pipeline if there is one already to avoid leaks */
  if (play) {
    gst_element_set_state(play, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(play));
    play = NULL;
  }

  /* Create and initialise a playbin element */
  play = gst_element_factory_make("playbin", "play");
  if (!play) return FALSE;
  g_object_set(G_OBJECT(play), "uri", uri, NULL);

  /* Connect the message bus callback to the playbin */
  gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(play)), bus_callback, NULL);

  return TRUE;
}


/* Attempt to load a file */
gboolean load_file(const gchar *uri) {
  if (build_gstreamer_pipeline(uri))
    return TRUE;

  return FALSE;
}


/* Attempt to play the loaded file */
gboolean play_file() {

  if (play) {
    /* Start playing */
    gst_element_set_state(play, GST_STATE_PLAYING);
    gtk_widget_set_sensitive(GTK_WIDGET(stop_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(pause_button), TRUE);
    /* Connect a callback to trigger every 200 milliseconds to
     * update the GUI with the playback progress. We remember
     * the ID of this source so that we can remove it when we stop
     * playing */
    timeout_source = g_timeout_add(200, (GSourceFunc)update_time_callback, play);
    return TRUE;
  }

  return FALSE;
}


/* Stop playing a file, if we're playing one. */


/* Attempt to seek to the given percentage through the file */
void seek_to(gdouble percentage)
{
  GstFormat fmt = GST_FORMAT_TIME;
  gint64 length;

  /* If it seems safe to attempt a seek... */
  if (play && gst_element_query_duration(play, &fmt, &length)) {
    /* ...calculate where to seek to */
    gint64 target = ((gdouble)length * (percentage / 100.0));

    /* ...and attempt the seek */
    if (!gst_element_seek(play, 1.0, GST_FORMAT_TIME,
	  GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET,
	  target, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
      g_print("Failed to seek to desired position\n");
  }
}

void initgui()
{
      gtk_widget_set_sensitive(GTK_WIDGET(stop_button), FALSE);
      gtk_widget_set_sensitive(GTK_WIDGET(pause_button), FALSE);
      gtk_range_set_value(GTK_RANGE(seek_scale), 0.0);
      gtk_label_set_text(GTK_LABEL(time_label), "--:--:--");
}
