/*
 * Copyright (C) 2010-2013 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MOTOSH_H__
#define __MOTOSH_H__

#include <linux/cdev.h>
#include <linux/i2c.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/switch.h>
#include <linux/wakelock.h>

/* The user-space-visible definitions. */
#include <uapi/linux/motosh.h>

#ifdef CONFIG_MMI_HALL_NOTIFICATIONS
#include <linux/mmi_hall_notifier.h>
#endif

#ifdef CONFIG_SENSORS_MOTOSH_MOTODISP
#include <linux/fb_quickdraw.h>
#include <linux/pm_qos.h>
#if defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>
#endif
#endif /* CONFIG_SENSORS_MOTOSH_MOTODISP */

#ifdef CONFIG_CYPRESS_CAPSENSE_HSSP
#include <linux/power_supply.h>
#endif

#define NAME			     "motosh"

#define MOTOSH_REG_SIZE 0x01

/* MOTOSH memory map.
 * We want to expand all entries, so we define all pre-processor guards if not
 * already defined, and at the end we undefine them if needed. */

#ifndef PDISPLAY
#define PDISPLAY
#define UNDEF_PDISPLAY
#endif

#ifndef IRGESTURE
#define IRGESTURE
#define UNDEF_IRGESTURE
#endif

#ifndef DSP
#define DSP
#define UNDEF_DSP
#endif

#ifndef GYRO_CALIBRATION
#define GYRO_CALIBRATION
#define UNDEF_GYRO_CALIBRATION
#endif

#ifndef CAPSENSE
#define CAPSENSE
#define UNDEF_CAPSENSE
#endif

#ifndef ALS_TEST
#define ALS_TEST
#define UNDEF_ALS_TEST
#endif

#define VMM_ENTRY(reg, id, writable, addr, size) id,
enum vmm_ids {
#include <linux/motosh_vmm.h>
};
#undef VMM_ENTRY

#ifdef UNDEF_ALS_TEST
#undef ALS_TEST
#endif

#ifdef UNDEF_CAPSENSE
#undef CAPSENSE
#endif

#ifdef UNDEF_GYRO_CALIBRATION
#undef GYRO_CALIBRATION
#endif

#ifdef UNDEF_DSP
#undef DSP
#endif

#ifdef UNDEF_IRGESTURE
#undef IRGESTURE
#endif

#ifdef UNDEF_PDISPLAY
#undef PDISPLAY
#endif

/* MOTOSH memory map end */

/* nwake interrupt mask */
#define N_DISP_ROTATE            0x01
#define N_ALS                    0x02
#define N_DISP_BRIGHTNESS        0x04
#define N_UPDATE_ACCEL_CAL       0x10
#define N_MOTO_MOD_CURRENT_DRAIN 0x20
#define N_STEP_COUNTER           0x80

#define MOTOSH_MAX_EVENT_QUEUE_SIZE   248
#define MOTOSH_UNKNOWN_MSG 0xffff

/* max size of total queue */
#define MOTOSH_MAX_NWAKE_EVENT_QUEUE_SIZE   5300
/* I2C DMA has a 3840 byte limit */
#define MOTOSH_MAX_NWAKE_EVENT_QUEUE_READ_SIZE 2650

#define MOTOSH_EVENT_QUEUE_MSG_ID_LEN 1
#define MOTOSH_EVENT_TIMESTAMP_LEN    3

#define LIGHTING_TABLE_SIZE 32

#define MOTOSH_MS_DATA_QUEUE_SIZE       0x08
#define MOTOSH_MS_DATA_QUEUE_MASK       0x07

#define MOTOSH_CLIENT_MASK		0xF0

#define MOTOSH_BUSY_STATUS_MASK	0x80
#define MOTOSH_BUSY_SLEEP_USEC	10000
#define MOTOSH_BUSY_RESUME_COUNT	14
#define MOTOSH_BUSY_SUSPEND_COUNT	6
#define MOTOSH_LATE_SUSPEND_TIMEOUT	400

#define AOD_WAKEUP_REASON_ESD		4
#define AOD_WAKEUP_REASON_QP_PREPARE		5
#define AOD_WAKEUP_REASON_QP_DRAW		6
#define AOD_WAKEUP_REASON_QP_ERASE		7
#define AOD_WAKEUP_REASON_QP_COMPLETE		8
#define AOD_WAKEUP_REASON_QP_DUMP_TRACE		9

#define AOD_QP_ACK_BUFFER_ID_MASK	0x3F
#define AOD_QP_ACK_RCVD			0
#define AOD_QP_ACK_DONE			1
#define AOD_QP_ACK_INVALID		2
#define AOD_QP_ACK_ESD_RECOVERED	3

#define AOD_QP_DRAW_MAX_BUFFER_ID	63
#define AOD_QP_DRAW_NO_OVERRIDE		0xFFFF

#define AOD_QP_ENABLED_VOTE_KERN		0x01
#define AOD_QP_ENABLED_VOTE_USER		0x02
#define AOD_QP_ENABLED_VOTE_MASK		0x03

/* Note: MAX_LOG_MSG_LEN must be LESS than MOTOSH_MAXDATA_LENGTH */
#define MAX_LOG_MSG_LEN			128

#define I2C_RESPONSE_LENGTH		8

#define MOTOSH_HEADER_LENGTH        1
#define MOTOSH_FOOTER_LENGTH        1
#define MOTOSH_MAX_PACKET_LENGTH    \
	(MOTOSH_HEADER_LENGTH + MOTOSH_MAXDATA_LENGTH + MOTOSH_FOOTER_LENGTH)

#define MOTOSH_IR_GESTURE_CNT      8
#define MOTOSH_IR_SZ_GESTURE       4
#define MOTOSH_IR_SZ_RAW           20
#define MOTOSH_IR_CONFIG_REG_SIZE  255

/* readbuff offsets */
#define ZERO_OFFSET  0

#define IRQ_WAKE_LO  0
#define IRQ_WAKE_MED 1
#define IRQ_WAKE_HI  2

#define IRQ_NOWAKE_LO   0
#define IRQ_NOWAKE_MED  1
#define IRQ_NOWAKE_HI   2

#define DOCK_STATE	0
#define PROX_DISTANCE	0
#define PROX_ALS        1
#define PROX_STOWED     3
#define PROX_RAW        4
#define PROX_NOISE_FLOOR 6
#define PROX_RECAL_THRESH 8
#define PROX_LOW_THRESH 10
#define PROX_HIGH_THRESH 12
#define COVER_STATE	0
#define TOUCH_REASON	1
#define FLAT_UP		0
#define FLAT_DOWN	0
#define STOWED_STATUS	0
#define STOWED_ALS		1
#define STOWED_PROX		3
#define STOWED_PROX_RAW 4
#define STOWED_NOISE_FLOOR 6
#define STOWED_RECAL_THRESH 8
#define STOWED_LOW_THRESH 10
#define STOWED_HIGH_THRESH 12
#define NFC_VALUE	0
#define ALGO_TYPE	7
#define COMPASS_STATUS	12
#define DISP_VALUE	0
#define ACCEL_RD_X	0
#define ACCEL_RD_Y	2
#define ACCEL_RD_Z	4
#define MAG_X		0
#define MAG_Y		2
#define MAG_Z		4
#define MAG_UNCALIB_X   6
#define MAG_UNCALIB_Y   8
#define MAG_UNCALIB_Z   10
#define ORIENT_X	6
#define ORIENT_Y	8
#define ORIENT_Z	10
#define QUAT_6AXIS_A	0
#define QUAT_6AXIS_B	2
#define QUAT_6AXIS_C	4
#define QUAT_6AXIS_W	6
#define QUAT_9AXIS_A	8
#define QUAT_9AXIS_B	10
#define QUAT_9AXIS_C	12
#define QUAT_9AXIS_W	14
#define GAME_RV_A       0
#define GAME_RV_B       2
#define GAME_RV_C       4
#define GAME_RV_W       6
#define GYRO_RD_X	0
#define GYRO_RD_Y	2
#define GYRO_RD_Z	4
#define GYRO_UNCALIB_X	6
#define GYRO_UNCALIB_Y	8
#define GYRO_UNCALIB_Z	10
#define ALS_VALUE	0
#define PRESSURE_VALUE	0
#define GRAV_X		0
#define GRAV_Y		2
#define GRAV_Z		4
#define CAMERA_VALUE	0
#define IR_GESTURE_EVENT    0
#define IR_GESTURE_ID       1
#define IR_STATE_STATE  0
#define STEP8_DATA	0
#define STEP16_DATA	2
#define STEP32_DATA	4
#define STEP64_DATA	6
#define SIM_OFFSET	0
#define STEP_DETECT	0
#define CHOPCHOP_DATA_OFFSET   0
#define LIFT_DISTANCE	0
#define LIFT_ROTATION	4
#define LIFT_GRAV_DIFF	8
#ifdef CONFIG_SENSORS_MOTOSH_HEADSET
#define HEADSET_VALUE   0
#endif /* CONFIG_SENSORS_MOTOSH_HEADSET */

#ifdef CONFIG_CYPRESS_CAPSENSE_HSSP
/* motosh_g_antcap_enabled */
#define ANTCAP_ENABLED             0x01
#define ANTCAP_AIRPLANE            0x02
#define ANTCAP_CHECK_CAL           0x04

/* motosh_g_conn_state */
#define ANTCAP_USB                 0x80
#define ANTCAP_HEADSET             0x40
#define ANTCAP_APM                 0x20

/* ANTCAP_CTRL, byte 0 */
#define ANTCAP_CTRL0_ENABLE        0x80
#define ANTCAP_CTRL0_RESET         0x40
#define ANTCAP_CTRL0_GETCFG        0x20
#define ANTCAP_CTRL0_SETCFG        0x10
#define ANTCAP_CTRL0_FLASH         0x08
#define ANTCAP_CTRL0_CAL           0x04
#define ANTCAP_CTRL0_DISABLE       0x00

/* ANTCAP_CTRL, byte 1 */
#define ANTCAP_CTRL1_USB           ANTCAP_USB
#define ANTCAP_CTRL1_HEADSET       ANTCAP_HEADSET

#define ANTCAP_CTRL1_NULL          0

#define ANTCAP_CTRL1_GET_INTF      1
#define ANTCAP_CTRL1_GET_VERS      2
#define ANTCAP_CTRL1_GET_VALS      3
#define ANTCAP_CTRL1_GET_CAL       5
#define ANTCAP_CTRL1_GET_FILT      6

#define ANTCAP_CTRL1_SET_INTF      1
#define ANTCAP_CTRL1_SET_VALS      3
#define ANTCAP_CTRL1_SET_CAL       5
#define ANTCAP_CTRL1_SET_FILT      6
#define ANTCAP_CTRL1_SET_CALFLASH  7
#endif

#define MS_TO_NS(msec) ((msec) * 1000 * 1000)

/* The following macros are intended to be called with the stm IRQ handlers */
/* only and refer to local variables in those functions. */
#define STMU16_TO_HOST(buf, x) ((uint16_t) be16_to_cpu(*((u16 *) (buf+(x)))))
#define STM16_TO_HOST(buf, x) ((int16_t) be16_to_cpu(*((u16 *) (buf+(x)))))
#define STM32_TO_HOST(buf, x) ((int32_t) be32_to_cpu(*((u32 *) (buf+(x)))))

#define MOTOSH_HALL_NO_DETECT 0
#define MOTOSH_HALL_SOUTH_DETECT 1
#define MOTOSH_HALL_NORTH_DETECT 2
#define MOTOSH_HALL_NORTH_OR_SOUTH_DETECT 3

#ifdef CONFIG_SENSORS_MOTOSH_HEADSET
#define SH_HEADSET_EMPTY          0x00
#define SH_HEADPHONE_DETECTED     0x01
#define SH_HEADSET_DETECTED       0x02
#define SH_HEADSET_BUTTON_1_DOWN  0x04
#define SH_HEADSET_BUTTON_2_DOWN  0x08
#define SH_HEADSET_BUTTON_3_DOWN  0x10
#define SH_HEADSET_BUTTON_4_DOWN  0x20
#endif /* CONFIG_SENSORS_MOTOSH_HEADSET */

#ifdef CONFIG_SENSORS_MOTOSH_MOTODISP
enum check_touch_type {
	NORMAL_CHECK,
	FORCE_UPDATE
};

struct motosh_quickpeek_message {
	u8 message;
	u8 panel_state;
	u8 buffer_id;
	u8 commit;
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;
	struct list_head list;
};

struct motosh_aod_enabled_vote {
	struct mutex vote_lock;
	unsigned int vote;
	unsigned int resolved_vote;
};
#endif /* CONFIG_SENSORS_MOTOSH_MOTODISP */

/* Note that this must match the sensorhub array */
struct motosh_derived_sens_rates {
	uint8_t rv_6axis_delay;
	uint8_t rv_9axis_delay;
	uint8_t game_rv_delay;
	uint8_t gravity_delay;
	uint8_t lin_accel_delay;
};

struct motosh_platform_data {
	int (*init)(void);
	void (*exit)(void);
	int (*power_on)(void);
	int (*power_off)(void);
	int gpio_reset;
	int gpio_bslen;
	int gpio_wakeirq;
	int gpio_int;
	int gpio_sh_wake;
	int gpio_sh_wake_resp;
	unsigned int bslen_pin_active_value;
	u16 lux_table[LIGHTING_TABLE_SIZE];
	u8 brightness_table[LIGHTING_TABLE_SIZE];
#ifdef CONFIG_SENSORS_MOTOSH_MOTODISP
	int qd_pm_qos_latency;
	unsigned int qd_pm_qos_timeout;
	int aod_touch_mode;
#endif /* CONFIG_SENSORS_MOTOSH_MOTODISP */
	char fw_version[FW_VERSION_SIZE];
	char fw_version_str[FW_VERSION_STR_MAX_LEN];
	int ct406_detect_threshold;
	int ct406_undetect_threshold;
	int ct406_recalibrate_threshold;
	int ct406_pulse_count;
	int ct406_prox_gain;
	int ct406_als_lux1_c0_mult;
	int ct406_als_lux1_c1_mult;
	int ct406_als_lux1_div;
	int ct406_als_lux2_c0_mult;
	int ct406_als_lux2_c1_mult;
	int ct406_als_lux2_div;
	int accel_orient;
	int gyro_orient;
	int mag_orient;
	int mag_config;
	int panel_type;
	int IR_config;
	int cover_detect_polarity;
#ifdef CONFIG_SENSORS_MOTOSH_HEADSET
	int headset_detect_enable;
	int headset_insertion_debounce;
	int headset_removal_debounce;
	int headset_button_down_debounce;
	int headset_button_up_debounce;
	int headset_button_0_1_threshold;
	int headset_button_1_2_threshold;
	int headset_button_2_3_threshold;
	int headset_button_3_upper_threshold;
	int headset_button_1_keycode;
	int headset_button_2_keycode;
	int headset_button_3_keycode;
	int headset_button_4_keycode;
#endif /* CONFIG_SENSORS_MOTOSH_HEADSET */
};

/**
 * as_node - Android sensor event node type
 * @list: underlying linux list_head structure
 * @data: data from the sensor event
 *
 * The as event queue is a list of these nodes
 */
struct as_node {
	struct list_head list;
	struct motosh_android_sensor_data data;
};

/* bitmask for batching flush requests */
#define FLUSH_ACCEL_REQ 0x00000001

/* bitmask for is_batching */
#define ACCEL_BATCHING 0x00000001

struct motosh_data {
	struct i2c_client *client;
	struct motosh_platform_data *pdata;
	/* to avoid two i2c communications at the same time */
	struct mutex lock;

	struct kthread_worker irq_worker;
	struct task_struct *irq_task;
	struct kthread_work irq_work;

	struct kthread_worker wake_irq_worker;
	struct task_struct *wake_irq_task;
	struct kthread_work wake_irq_work;

	unsigned int wake_work_delay;
	struct wake_lock wakelock;
	struct wake_lock reset_wakelock;
	struct input_dev *input_dev;

	struct mutex sh_wakeup_lock;

	int hw_initialized;

	atomic_t enabled;
	int irq;
	int irq_wake;
	unsigned int current_addr;
	enum stm_mode mode;
	unsigned char intp_mask;

	dev_t motosh_dev_num;
	struct class *motosh_class;
	struct device *motosh_class_dev;
	struct cdev as_cdev;
	struct cdev ms_cdev;

	struct switch_dev dsdev; /* Standard Dock switch */
	struct switch_dev edsdev; /* Motorola Dock switch */

	/* Android sensor event queue */
	struct as_node as_queue;
	/* Lock for modifying as_queue */
	spinlock_t as_queue_lock;

	wait_queue_head_t motosh_as_data_wq;

	struct motosh_moto_sensor_data
		motosh_ms_data_buffer[MOTOSH_MS_DATA_QUEUE_SIZE];
	int motosh_ms_data_buffer_head;
	int motosh_ms_data_buffer_tail;
	wait_queue_head_t motosh_ms_data_wq;

	struct regulator *regulator_1;
	struct regulator *regulator_2;

#ifdef CONFIG_SENSORS_MOTOSH_MOTODISP
	/* Quick peek data */
	struct workqueue_struct *quickpeek_work_queue;
	struct work_struct quickpeek_work;
	struct wake_lock quickpeek_wakelock;
	struct list_head quickpeek_command_list;
	wait_queue_head_t quickpeek_wait_queue;
	atomic_t qp_enabled;
	bool quickpeek_occurred;
	unsigned long qw_irq_status;
	struct motosh_aod_enabled_vote aod_enabled;
	bool qp_in_progress;
	bool qp_prepared;
	struct mutex qp_list_lock;
	struct pm_qos_request pm_qos_req_dma;
#endif /* CONFIG_SENSORS_MOTOSH_MOTODISP */

	bool ignore_wakeable_interrupts;
	int ignored_interrupts;

	bool in_reset_and_init;
	bool is_suspended;
	bool resume_cleanup;
	/* bitmask to invoke flush complete for all pending sensor requests */
	uint32_t nwake_flush_req;
	/* bitmask to track sensors being batched */
	uint32_t is_batching;
	bool pending_wake_work;
	bool vr_mode;
#if defined(CONFIG_FB)
	struct notifier_block fb_notif;
#endif
	struct mmi_hall_data *hall_data;
#ifdef CONFIG_CYPRESS_CAPSENSE_HSSP
	struct power_supply antcap_psy;
#endif
};

/* per algo config, request, and event registers */
struct motosh_algo_info_t {
	unsigned short config_bit;
	unsigned char cfg_register;
	unsigned char req_register;
	unsigned char evt_register;
	unsigned short evt_size;
};

#define ALGO_RQST_DATA_SIZE 28
struct motosh_algo_requst_t {
	char size;
	char data[ALGO_RQST_DATA_SIZE];
};

int64_t motosh_timestamp_ns(void);
int motosh_set_derived_sens_update_rate(struct motosh_data *ps_motosh);


irqreturn_t motosh_isr(int irq, void *dev);
void motosh_irq_thread_func(struct kthread_work *work);

irqreturn_t motosh_wake_isr(int irq, void *dev);
void motosh_irq_wake_thread_func(struct kthread_work *work);
int motosh_process_ir_gesture(struct motosh_data *ps_motosh,
	unsigned char *data);

long motosh_misc_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg);

void motosh_reset(struct motosh_platform_data *pdata);
int motosh_reset_and_init(enum reset_mode mode);

int motosh_as_data_buffer_write(struct motosh_data *ps_motosh,
	unsigned char type, unsigned char *data, int size,
	unsigned char status, unsigned char *timestamp);
int motosh_as_data_buffer_read(struct motosh_data *ps_motosh,
	struct motosh_android_sensor_data *buff);
int motosh_ms_data_buffer_write(struct motosh_data *ps_motosh,
	unsigned char type, unsigned char *data, int size, bool timestamped);
int motosh_ms_data_buffer_read(struct motosh_data *ps_motosh,
	struct motosh_moto_sensor_data *buff);

/* motosh_i2c_write_read_no_reset()
 */
int motosh_i2c_write_read_no_reset(struct motosh_data *ps_motosh,
	u8 *writebuff,
	u8 *readbuff,
	int writelen,
	int readlen);
/* motosh_i2c_read_no_reset()
 */
int motosh_i2c_read_no_reset(struct motosh_data *ps_motosh,
	u8 *buf, int len);
/* motosh_i2c_write_no_reset()
 */
int motosh_i2c_write_no_reset(struct motosh_data *ps_motosh,
	u8 *buf, int len);
/* motosh_i2c_write_read()
 */
int motosh_i2c_write_read(struct motosh_data *ps_motosh,
	u8 *writebuff,
	u8 *readbuff,
	int writelen,
	int readlen);
/* motosh_i2c_read()
 */
int motosh_i2c_read(struct motosh_data *ps_motosh, u8 *buf, int len);
/* motosh_i2c_write()
 */
int motosh_i2c_write(struct motosh_data *ps_motosh, u8 *buf, int len);
int motosh_enable(struct motosh_data *ps_motosh);

void motosh_wake(struct motosh_data *ps_motosh);
void motosh_sleep(struct motosh_data *ps_motosh);
void motosh_detect_lowpower_mode(unsigned char *cmdbuff);

int motosh_load_brightness_table(struct motosh_data *ps_motosh,
	unsigned char *cmdbuff);

int motosh_irq_wake_work_func_display_locked(struct motosh_data *ps_motosh,
	unsigned short irq_status);
unsigned short motosh_get_interrupt_status(struct motosh_data *ps_motosh,
	unsigned char reg, int *err);
#ifdef CONFIG_SENSORS_MOTOSH_MOTODISP
void motosh_quickpeek_work_func(struct work_struct *work);
void motosh_quickpeek_reset_locked(struct motosh_data *ps_motosh);
int motosh_quickpeek_disable_when_idle(struct motosh_data *ps_motosh);
void motosh_vote_aod_enabled_locked(struct motosh_data *ps_motosh, int voter,
	bool enable);
void motosh_store_vote_aod_enabled(struct motosh_data *ps_motosh, int voter,
	bool enable);
void motosh_store_vote_aod_enabled_locked(struct motosh_data *ps_motosh,
	int voter, bool enable);
int motosh_resolve_aod_enabled_locked(struct motosh_data *ps_motosh);
int motosh_display_handle_touch_locked(struct motosh_data *ps_motosh);
int motosh_display_handle_quickpeek_locked(struct motosh_data *ps_motosh);
void motosh_quickwakeup_init(struct motosh_data *ps_motosh);

int motosh_check_touch_config_locked(enum check_touch_type);
#endif /* CONFIG_SENSORS_MOTOSH_MOTODISP */

int motosh_boot_flash_erase(void);
int motosh_get_version(struct motosh_data *ps_motosh);
int motosh_get_version_str(struct motosh_data *ps_motosh);
int switch_motosh_mode(enum stm_mode mode);
int motosh_bootloadermode(struct motosh_data *ps_motosh);

void motosh_time_sync(void);
int64_t motosh_time_recover(int32_t hubshort, int64_t cur_time);
int motosh_time_drift_comp(int64_t rec_hub, int64_t cur_time, bool streaming);

void motosh_time_compare(void);

extern struct motosh_data *motosh_misc_data;

extern struct motosh_moto_sensor_batch_cfg motosh_g_acc_cfg;
extern unsigned short motosh_g_mag_delay;
extern unsigned short motosh_g_gyro_delay;
extern unsigned short motosh_g_baro_delay;
extern unsigned short motosh_g_als_delay;
extern struct motosh_derived_sens_rates motosh_g_derived_sens_rates;
extern unsigned short motosh_g_ir_gesture_delay;
extern unsigned short motosh_g_ir_raw_delay;
extern unsigned short motosh_g_step_counter_delay;
extern unsigned long motosh_g_nonwake_sensor_state;
extern unsigned short motosh_g_algo_state;
extern unsigned char motosh_g_motion_dur;
extern unsigned char motosh_g_zmotion_dur;
extern unsigned char motosh_g_control_reg[MOTOSH_CONTROL_REG_SIZE];
extern unsigned char motosh_g_mag_cal[MOTOSH_MAG_CAL_SIZE];
extern unsigned char motosh_g_gyro_cal[MOTOSH_GYRO_CAL_SIZE];
extern unsigned char motosh_g_accel_cal[MOTOSH_ACCEL_CAL_SIZE];
extern unsigned short motosh_g_control_reg_restore;
extern unsigned char motosh_g_ir_config_reg[MOTOSH_IR_CONFIG_REG_SIZE];
extern bool motosh_g_ir_config_reg_restore;
extern bool motosh_g_booted;

#ifdef CONFIG_CYPRESS_CAPSENSE_HSSP
extern unsigned char motosh_g_antcap_cal[MOTOSH_ANTCAP_CAL_BUFF_SIZE];
extern unsigned char motosh_g_antcap_cfg[MOTOSH_ANTCAP_CFG_BUFF_SIZE];
extern unsigned char motosh_g_conn_state;
extern unsigned char motosh_g_antcap_enabled;
extern unsigned char motosh_g_antcap_hw_ready;
extern unsigned char motosh_g_antcap_sw_ready;

extern int motosh_antcap_register(void);
extern int motosh_antcap_of_init(struct i2c_client *);
extern int motosh_antcap_i2c_send_enable(unsigned char);
extern int motosh_antcap_i2c_getcal_poll(unsigned char);
extern int motosh_antcap_i2c_setcal_poll(unsigned char);
extern int motosh_antcap_i2c_getver_poll(unsigned char);
extern int motosh_antcap_i2c_flash_poll(unsigned char);
extern int motosh_antcap_check_cal(void);
extern int motosh_antcap_read_cal(void);
#endif

extern unsigned short motosh_i2c_retry_delay;

extern const struct motosh_algo_info_t motosh_algo_info[];

extern struct motosh_algo_requst_t motosh_g_algo_requst[];

extern long motosh_time_delta;

extern unsigned int motosh_irq_disable;

extern unsigned long motosh_g_wake_sensor_state;

extern unsigned char stat_string[];

extern const struct file_operations motosh_as_fops;
extern const struct file_operations motosh_ms_fops;
extern const struct file_operations motosh_misc_fops;
extern struct miscdevice motosh_misc_device;

#endif  /* __MOTOSH_H__ */
