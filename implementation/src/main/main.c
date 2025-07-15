/*
    AUTHOR: Ronny N. Ismael
    DATE: 2025-07-0

    DESCRIPTION: Module that will control a dog door on a cat litter box
    to keep my dumb dog out of the litter box because he loves to eat the
    cat poop.
*/

#include "esp_log.h"
#include "nvs_flash.h"
/* BLE */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"

#define RSSI_THRESHOLD (-55) /* Placeholder for now, gotta experiment to figure it out */

static const char *TAG = "BLE_ProxiGate";
static const uint8_t MY_TAG_ADDR[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  /* MAC Address of BLE beacon */


static void ble_addr_to_str_fixed(const ble_addr_t *addr, char *str, size_t size)
{
    snprintf(str, size,
             "%02X:%02X:%02X:%02X:%02X:%02X",
             addr->val[5], addr->val[4], addr->val[3],
             addr->val[2], addr->val[1], addr->val[0]);
}

/* Called when a device is discovered during scanning */
static int ble_gap_event_cb(struct ble_gap_event *event, void *arg)
{
    if (event->type == BLE_GAP_EVENT_DISC)
    {
        char addr_str[18] = {0};
        ble_addr_to_str_fixed(&event->disc.addr, addr_str, sizeof(addr_str));
        /* ESP_LOGI(TAG, "Found device: %s, RSSI: %d", addr_str, event->disc.rssi); */

        /* if mac address matches do xyz */
        /*
         * Probably depreciated if using rotating mac addressing
         */
        (void)MY_TAG_ADDR;
        // if (memcmp(event->disc.addr.val, MY_TAG_ADDR, 6) == 0 &&
        //     event->disc.rssi > RSSI_THRESHOLD)
        // {
        //     ESP_LOGI(TAG, ">>> Target device within ~1m detected!");
        //     /* call handler */
        // }

        struct ble_hs_adv_fields fields;
        if (ble_hs_adv_parse_fields(&fields, event->disc.data, event->disc.length_data) == 0) {
            if (fields.name != NULL)
            {
                /* ESP_LOGI(TAG, "Device name: %.*s", fields.name_len, fields.name); */

                if (strncmp((const char*)fields.name, "Ronny", fields.name_len) == 0)
                {
                    ESP_LOGI(TAG, ">>> UUID: %s, RSSI: %d", fields.name, event->disc.rssi);
                }
            }
        }
    }
    return 0;
}

/* Start scanning */
static void ble_app_scan(void)
{
    struct ble_gap_disc_params params = {0};

    params.passive = 0;       /* active scanning */
    params.itvl = 0x0010;     /* 10ms */
    params.window = 0x0010;   /* 10ms */
    params.filter_policy = 0;
    params.limited = 0;

    int rc = ble_gap_disc(0, BLE_HS_FOREVER, &params, ble_gap_event_cb, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "Error initiating GAP discovery: %d", rc);
    } else
    {
        ESP_LOGI(TAG, "Scanning started...");
    }
}

/* BLE host task */
void host_task(void *param)
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
    if  (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Initialize the BLE stack */
    ret = nimble_port_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to init nimble %d ", ret);
        return;
    }

    ble_hs_cfg.reset_cb = NULL;
    ble_hs_cfg.sync_cb = ble_app_scan;  /* called when BLE stack is ready */

    nimble_port_freertos_init(host_task);
}
