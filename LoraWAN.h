

#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"

#include "mbed.h"
#include "trace_helper.h"


/*
 * Sets up an application dependent transmission timer in ms. Used only when Duty Cycling is off for testing
 */
#define TX_TIMER 10000

/**
 * Maximum number of events for the event queue.
 * 10 is the safe number for the stack events, however, if application
 * also uses the queue for whatever purposes, this number should be increased.
 */
#define MAX_NUMBER_OF_EVENTS 10

/**
 * Maximum number of retries for CONFIRMED messages before giving up
 */
#define CONFIRMED_MSG_RETRY_COUNTER 3


class LoraWAN
{
public:

    LoraWAN();
    int init_lora();
    int connect_lora();
    void send_message();
    void  receive_message();
    int disconnect_lora();

    void init_callback(Callback<void(uint8_t*)> fonction);

    

private:

    uint8_t tx_buffer[30];
    uint8_t rx_buffer[30];
    /**
    * This event queue is the global event queue for both the
    * application and stack. To conserve memory, the stack is designed to run
    * in the same thread as the application and the application is responsible for
    * providing an event queue to the stack that will be used for ISR deferment as
    * well as application information event queuing.
    */
    EventQueue *ev_queue;

    /**
     * Event handler.
     *
     * This will be passed to the LoRaWAN stack to queue events for the
     * application which in turn drive the application.
     */
    void lora_event_handler(lorawan_event_t event);

    /**
     * Constructing Mbed LoRaWANInterface and passing it the radio object from lora_radio_helper.
     */
    LoRaWANInterface *lorawan;

    /**
     * Application specific callbacks
     */
    lorawan_app_callbacks_t callbacks;

    Callback<void(uint8_t*)> fonction_callback;
    

};