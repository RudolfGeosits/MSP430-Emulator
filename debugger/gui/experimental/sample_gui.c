#include <gtk/gtk.h>

GtkWidget *window = NULL;
GtkWidget *main_image = NULL;
GtkWidget *power_led_image = NULL;

static gboolean main_loop(GtkWidget *widget)
{
  static gboolean show = TRUE;
  if (widget->window == NULL) return FALSE;

  if (show) {
    gtk_widget_show(main_image);
  }
  else {
    gtk_widget_hide(main_image);
  }

  show = !show;
  g_print("sss\n");

  return TRUE;
}


int main(int argc, char **argv)
{
  /* Initialize GTK */
  gtk_init(&argc, &argv);
  
  /* Creat Widgets */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "MSP430-Emulator");

  main_image = gtk_image_new_from_file("msp.png");
  power_led_image = gtk_image_new_from_file("power_led.png");

  if (main_image == NULL) {
    g_printerr("Could not open \"msp.png\"\n");
    return 1;
  }
  
  /* Window border */
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* attach standard event handlers */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  g_timeout_add(333, (GSourceFunc) main_loop, (gpointer) window);

  gtk_container_add(GTK_CONTAINER(window), main_image);
  //gtk_container_add(GTK_CONTAINER(window), power_led_image);
  gtk_widget_show_all(GTK_WIDGET(window));

  gtk_main();

  return 0;
}
