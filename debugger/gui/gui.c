/*                                                                            
  MSP430 Emulator                                                         
  Copyright (C) 2014, 2015 Rudolf Geosits (rgeosits@live.esu.edu)            
                                                                             
  This program is free software: you can redistribute it and/or modify    
  it under the terms of the GNU General Public License as published by  
  the Free Software Foundation, either version 3 of the License, or   
  (at your option) any later version.                                          
                                                                               
  This program is distributed in the hope that it will be useful,              
  but WITHOUT ANY WARRANTY; without even the implied warranty of    
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 
  GNU General Public License for more details.                                 
                                                                               
  You should have received a copy of the GNU General Public License            
  along with this program. If not, see <http://www.gnu.org/licenses            
*/

GtkWidget *window = NULL;
GtkWidget *msp_power_led = NULL;
GtkWidget *msp_no_power = NULL;
GtkWidget *msp_p1_0 = NULL;

static gboolean main_loop(GtkWidget *widget)
{
  static bool others_on = false;
  if (widget->window == NULL) return FALSE;

  if (P1DIR_0 && P1OUT_0) {
    gtk_image_set_from_file( (GtkImage *)msp_no_power, 
    			     "debugger/gui/msp_p1.0.png");  

    //puts("*RED ON\n");
    others_on = true;
  }
  
  if (P1DIR_6 && P1OUT_6) {
    gtk_image_set_from_file( (GtkImage *)msp_no_power, 
    			     "debugger/gui/msp_p1.6.png");      

    //puts("*GREEN ON\n");
    others_on = true;
  }

  if (P1DIR_0 && P1OUT_0 && P1DIR_6 && P1OUT_6) {
    gtk_image_set_from_file( (GtkImage *)msp_no_power, 
    			     "debugger/gui/msp_p1.0_p1.6.png");      
    //puts("*BOTH ON\n");
    others_on = true;    
  }
  
  if (!others_on) {
    gtk_image_set_from_file((GtkImage *)msp_no_power, 
  			    "debugger/gui/msp_power_on.png");
    others_on = false;
  }

  return TRUE;
}

void *gui(void *x)
{
  /* Initialize GTK */
  gtk_init(NULL, NULL);
  
  /* Creat Widgets */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "MSP430-Emulator");

  msp_power_led = gtk_image_new_from_file("debugger/gui/msp_power_on.png");
  msp_no_power = gtk_image_new_from_file("debugger/gui/msp_power_static.png");
  msp_p1_0 = gtk_image_new_from_file("debugger/gui/msp_p1.0.png");

  if (msp_no_power == NULL || msp_power_led == NULL) {
    g_print("Image not loaded\n");
    exit(1);
  }
  
  /* Window border */
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* attach standard event handlers */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  g_timeout_add(300, (GSourceFunc) main_loop, (gpointer) window);

  gtk_container_add(GTK_CONTAINER(window), msp_no_power);
  gtk_widget_show_all(GTK_WIDGET(window));

  gtk_main();
  return NULL;
}
