/*
    AUTHOR: Ronny N. Ismael
    DATE: 2025-07-0

    DESCRIPTION: Module that will control a dog door on a cat litter box
    to keep my dumb dog out of the litter box because he loves to eat the
    cat poop.
*/

#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"
#include "esp_err.h"
#include "servo_motor.h"
#include "soc/gpio_num.h"
#include <stdlib.h>

/*****************************
 *  DEFINES
 *****************************/

#define RSSI_THRESHOLD_CLOSE (-30)  /* below this, close */
#define RSSI_THRESHOLD_OPEN  (-45)  /* above this, open */

#define PASS (0)
#define FAIL (-1)

/*****************************
 *  GLOBALS
 *****************************/

static const char *TAG = "BLE_ProxiGate";
static const uint8_t MY_TAG_ADDR[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  /* MAC Address of TARGET BLE beacon */
static bool door_closed = false;

/*****************************
 *  FUNCTION PROTOTYPES
 *****************************/

/*****************************
 *  LOCAL FUNCTIONS
 *****************************/

static int
handle_proximity_detected(void)
{
    /* close motors */
    ESP_LOGI(TAG, ">>> Proximity threshold reached, closing door...");
    set_servo_angle(90);
    /* send ping over wifi? */
    /* blare alarm? */
    return PASS;
}

static int
handle_proximity_lost(void)
{
    /* if door is open then close? */
    ESP_LOGI(TAG, ">>> Proximity threshold lost, opening door...");
    set_servo_angle(0);
    return PASS;
}

static void
__attribute__((always_inline))
ble_addr_to_str_fixed(const ble_addr_t *addr, char *str, size_t size)
{
    snprintf(str, size,
             "%02X:%02X:%02X:%02X:%02X:%02X",
             addr->val[5], addr->val[4], addr->val[3],
             addr->val[2], addr->val[1], addr->val[0]);
}

/* Called when a device is discovered during scanning */
static int
__attribute__((flatten, hot))
ble_gap_event_cb(struct ble_gap_event *event, void *arg)
{
    int rc;
    (void) MY_TAG_ADDR;

    if (BLE_GAP_EVENT_DISC == event->type)
    {
        char addr_str[18] = {0};
        ble_addr_to_str_fixed(&event->disc.addr, addr_str, sizeof(addr_str));
        /* ESP_LOGI(TAG, "Found device: %s, RSSI: %d", addr_str, event->disc.rssi); */

        /* if mac address matches do xyz */
        /*
         * Probably depreciated if using rotating mac addressing
         */
        // if (memcmp(event->disc.addr.val, MY_TAG_ADDR, 6) == 0 &&
        //     event->disc.rssi > RSSI_THRESHOLD)
        // {
        //     ESP_LOGI(TAG, ">>> Target device within ~1m detected!");
        //     /* call handler */
        // }

        struct ble_hs_adv_fields fields;
        if (0 == ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data)) {
            if (NULL != fields.name)
            {
                /* ESP_LOGI(TAG, "Device name: %.*s", fields.name_len, fields.name); */
                if (0 == strncmp((const char*)fields.name, "Ronny", fields.name_len))
                {
                    ESP_LOGI(TAG, ">>> Name: %.*s, RSSI: %d", fields.name_len, fields.name, event->disc.rssi);
                    if (RSSI_THRESHOLD_CLOSE < event->disc.rssi)
                    {
                        if (!door_closed)
                        {
                            rc = handle_proximity_detected();
                            if (PASS != rc)
                            {
                                ESP_LOGE(TAG, "Error: handling handle_proximity_detected()");
                            }
                            door_closed = true;
                        }
                    } else if (RSSI_THRESHOLD_OPEN > event->disc.rssi)
                    {
                        if (door_closed)
                        {
                            rc = handle_proximity_lost();
                            if (PASS != rc)
                            {
                                ESP_LOGE(TAG, "Error: handling handle_proximity_lost()");
                            }
                            door_closed = false;
                        }
                    }
                }
            }
        }
    }
    return PASS;
}

/* Start scanning */
static void
ble_app_scan(void)
{
    int rc;
    struct ble_gap_disc_params params = {0};

    params.passive = 0;       /* active scanning */
    params.itvl = 0x0010;     /* 10ms */
    params.window = 0x0010;   /* 10ms */
    params.filter_policy = 0;
    params.limited = 0;

    rc = ble_gap_disc(0, BLE_HS_FOREVER, &params, ble_gap_event_cb, NULL);
    if (PASS != rc)
    {
        ESP_LOGE(TAG, "Error initiating GAP discovery: %d", rc);
    } else
    {
        ESP_LOGI(TAG, "Scanning started...");
    }
}

/*****************************
 *  GLOBAL FUNCTIONS
 *****************************/

/* BLE host task */
void
host_task(void *param)
{
    ESP_LOGI(TAG, "NimBLE Host task started");
    nimble_port_run();  /* This function will return only when nimble_port_stop() is called */
    nimble_port_freertos_deinit();
    /* vTaskDelay(100 / portTICK_PERIOD_MS); */
}

void
app_main(void)
{
    ESP_LOGI(TAG, "ESP32 startup procedure beginning...");

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if  (ESP_ERR_NVS_NO_FREE_PAGES == ret || ESP_ERR_NVS_NEW_VERSION_FOUND == ret)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Initialize the BLE stack */
    ret = nimble_port_init();
    if (ESP_OK != ret)
    {
        ESP_LOGE(TAG, "Failed to init nimble %d ", ret);
        return;
    }

    setup_pwm(GPIO_NUM_33);

    ble_hs_cfg.reset_cb = NULL;
    ble_hs_cfg.sync_cb = ble_app_scan;  /* called when BLE stack is ready */

    nimble_port_freertos_init(host_task);
}

