# 1 "canlib/message/msg_sensor.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "canlib/message/msg_sensor.c" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdbool.h" 1 3
# 1 "canlib/message/msg_sensor.c" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned __int24 uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/alltypes.h" 3
typedef __int24 intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\c99\\stdint.h" 2 3
# 2 "canlib/message/msg_sensor.c" 2


# 1 "canlib\\can.h" 1
# 14 "canlib\\can.h"
typedef uint32_t can_sid_t;


typedef struct {

    uint8_t brp;

    uint8_t sjw;


    uint8_t sam;

    uint8_t seg1ph;

    uint8_t seg2ph;

    uint8_t prseg;



    _Bool btlmode;
} can_timing_t;


typedef struct {

    can_sid_t sid;

    uint8_t data_len;

    uint8_t data[8];
} can_msg_t;
# 4 "canlib/message/msg_sensor.c" 2

# 1 "canlib\\message_types.h" 1






typedef enum {
    PRIO_HIGHEST = 0x0,
    PRIO_HIGH = 0x1,
    PRIO_MEDIUM = 0x2,
    PRIO_LOW = 0x3,
} can_msg_prio_t;


typedef enum {
    MSG_GENERAL_BOARD_STATUS = 0x001,
    MSG_RESET_CMD = 0x002,
    MSG_DEBUG_RAW = 0x003,
    MSG_CONFIG_SET = 0x004,
    MSG_CONFIG_STATUS = 0x005,
    MSG_ACTUATOR_CMD = 0x006,
    MSG_ACTUATOR_ANALOG_CMD = 0x007,
    MSG_ACTUATOR_STATUS = 0x008,
    MSG_ALT_ARM_CMD = 0x009,
    MSG_ALT_ARM_STATUS = 0x00A,
    MSG_SENSOR_TEMP = 0x00B,
    MSG_SENSOR_ALTITUDE = 0x00C,
    MSG_SENSOR_IMU_X = 0x00D,
    MSG_SENSOR_IMU_Y = 0x00E,
    MSG_SENSOR_IMU_Z = 0x00F,
    MSG_SENSOR_MAG_X = 0x010,
    MSG_SENSOR_MAG_Y = 0x011,
    MSG_SENSOR_MAG_Z = 0x012,
    MSG_SENSOR_ANALOG = 0x013,
    MSG_GPS_TIMESTAMP = 0x014,
    MSG_GPS_LATITUDE = 0x015,
    MSG_GPS_LONGITUDE = 0x016,
    MSG_GPS_ALTITUDE = 0x017,
    MSG_GPS_INFO = 0x018,
    MSG_STATE_EST_DATA = 0x019,
    MSG_LEDS_ON = 0x01A,
    MSG_LEDS_OFF = 0x01B,
} can_msg_type_t;


typedef enum {
    BOARD_TYPE_ID_ANY = 0x00,
    BOARD_TYPE_ID_INJ_SENSOR = 0x01,
    BOARD_TYPE_ID_CANARD_MOTOR = 0x02,
    BOARD_TYPE_ID_CAMERA = 0x03,
    BOARD_TYPE_ID_POWER = 0x04,
    BOARD_TYPE_ID_LOGGER = 0x05,
    BOARD_TYPE_ID_PROCESSOR = 0x06,
    BOARD_TYPE_ID_TELEMETRY = 0x07,
    BOARD_TYPE_ID_GPS = 0x08,
    BOARD_TYPE_ID_SRAD_GNSS = 0x09,
    BOARD_TYPE_ID_ALTIMETER = 0x0A,
    BOARD_TYPE_ID_ARMING = 0x0B,
    BOARD_TYPE_ID_PAY_SENSOR = 0x40,
    BOARD_TYPE_ID_PAY_MOTOR = 0x41,
    BOARD_TYPE_ID_RLCS_GLS = 0x80,
    BOARD_TYPE_ID_RLCS_RELAY = 0x81,
    BOARD_TYPE_ID_RLCS_HEATING = 0x82,
    BOARD_TYPE_ID_DAQ = 0x83,
    BOARD_TYPE_ID_CHARGING = 0x84,
    BOARD_TYPE_ID_THERMOCOUPLE = 0x85,
    BOARD_TYPE_ID_USB = 0x86,
    BOARD_TYPE_ID_FYDP25_TVCA = 0xC0,
} can_board_type_id_t;


typedef enum {
    BOARD_INST_ID_ANY = 0x00,
    BOARD_INST_ID_GENERIC = 0x01,
} can_board_inst_id_t;

typedef enum {
    BOARD_INST_ID_CANARD_MOTOR_PRIMARY = 0x02,
    BOARD_INST_ID_CANARD_MOTOR_FAILSAFE = 0x03,
} can_board_inst_id_canard_motor_t;

typedef enum {
    BOARD_INST_ID_CAMERA_INJ_A = 0x04,
    BOARD_INST_ID_CAMERA_INJ_B = 0x05,
    BOARD_INST_ID_CAMERA_VENT_A = 0x06,
    BOARD_INST_ID_CAMERA_VENT_B = 0x07,
    BOARD_INST_ID_CAMERA_VENT_C = 0x08,
    BOARD_INST_ID_CAMERA_VENT_D = 0x09,
    BOARD_INST_ID_CAMERA_RECOVERY = 0x0A,
} can_board_inst_id_camera_t;

typedef enum {
    BOARD_INST_ID_POWER_ROCKET = 0x0B,
    BOARD_INST_ID_POWER_PAYLOAD = 0x0C,
} can_board_inst_id_power_t;

typedef enum {
    BOARD_INST_ID_THERMOCOUPLE_1 = 0x0D,
    BOARD_INST_ID_THERMOCOUPLE_2 = 0x0E,
    BOARD_INST_ID_THERMOCOUPLE_3 = 0x0F,
    BOARD_INST_ID_THERMOCOUPLE_4 = 0x10,
} can_board_inst_id_thermocouple_t;

typedef enum {
    E_NOMINAL = 0x00,
    E_5V_OVER_CURRENT = 0x01,
    E_5V_OVER_VOLTAGE = 0x02,
    E_5V_UNDER_VOLTAGE = 0x04,
    E_12V_OVER_CURRENT = 0x08,
    E_12V_OVER_VOLTAGE = 0x10,
    E_12V_UNDER_VOLTAGE = 0x20,
    E_IO_ERROR = 0x40,
    E_FS_ERROR = 0x80,
} can_general_board_status_t;

typedef enum {
    ACTUATOR_OX_INJECTOR_VALVE = 0x00,
    ACTUATOR_FUEL_INJECTOR_VALVE = 0x01,
    ACTUATOR_CHARGE_ENABLE = 0x02,
    ACTUATOR_5V_RAIL_ROCKET = 0x03,
    ACTUATOR_5V_RAIL_PAYLOAD = 0x04,
    ACTUATOR_TELEMETRY = 0x05,
    ACTUATOR_CAMERA_INJ_A = 0x06,
    ACTUATOR_CAMERA_INJ_B = 0x07,
    ACTUATOR_CAMERA_VENT_A = 0x08,
    ACTUATOR_CAMERA_VENT_B = 0x09,
    ACTUATOR_CAMERA_VENT_C = 0x0A,
    ACTUATOR_CAMERA_VENT_D = 0x0B,
    ACTUATOR_CAMERA_RECOVERY = 0x0C,
    ACTUATOR_PROC_ESTIMATOR_INIT = 0x0D,
    ACTUATOR_CANARD_ENABLE = 0x0E,
    ACTUATOR_CANARD_ANGLE = 0x0F,
} can_actuator_id_t;

typedef enum {
    ACTUATOR_ON = 0x00,
    ACTUATOR_OFF = 0x01,
    ACTUATOR_UNK = 0x02,
    ACTUATOR_ILLEGAL = 0x03,
} can_actuator_state_t;

typedef enum {
    ALTIMETER_RAVEN = 0x00,
    ALTIMETER_STRATOLOGGER = 0x01,
    ALTIMETER_SRAD = 0x02,
} can_altimeter_id_t;

typedef enum {
    ALT_ARM_STATE_DISARMED = 0x00,
    ALT_ARM_STATE_ARMED = 0x01,
} can_alt_arm_state_t;

typedef enum {
    IMU_PROC_ALTIMU10 = 0x00,
    IMU_PROC_MTI630 = 0x01,
    IMU_PROC_LSM6DSO32 = 0x02,
    IMU_SRAD_ALT_ALTIMU10 = 0x03,
} can_imu_id_t;

typedef enum {
    SENSOR_5V_VOLT = 0x00,
    SENSOR_5V_CURR = 0x01,
    SENSOR_12V_VOLT = 0x02,
    SENSOR_12V_CURR = 0x03,
    SENSOR_CHARGE_VOLT = 0x04,
    SENSOR_CHARGE_CURR = 0x05,
    SENSOR_BATT_VOLT = 0x06,
    SENSOR_BATT_CURR = 0x07,
    SENSOR_MOTOR_CURR = 0x08,
    SENSOR_PRESSURE_OX = 0x09,
    SENSOR_PRESSURE_FUEL = 0x0A,
    SENSOR_PRESSURE_CC = 0x0B,
    SENSOR_BARO_PRESSURE = 0x0C,
    SENSOR_BARO_TEMP = 0x0D,
    SENSOR_RA_BATT_VOLT_1 = 0x0E,
    SENSOR_RA_BATT_VOLT_2 = 0x0F,
    SENSOR_RA_BATT_CURR_1 = 0x10,
    SENSOR_RA_BATT_CURR_2 = 0x11,
    SENSOR_RA_MAG_VOLT_1 = 0x12,
    SENSOR_RA_MAG_VOLT_2 = 0x13,
    SENSOR_FPS = 0x14,
    SENSOR_CANARD_ENCODER_1 = 0x15,
    SENSOR_CANARD_ENCODER_2 = 0x16,
    SENSOR_PROC_FLIGHT_PHASE_STATUS = 0x17,
    SENSOR_VELOCITY = 0x18,
} can_analog_sensor_id_t;

typedef enum {
    STATE_ID_ATT_Q0 = 0x00,
    STATE_ID_ATT_Q1 = 0x01,
    STATE_ID_ATT_Q2 = 0x02,
    STATE_ID_ATT_Q3 = 0x03,
    STATE_ID_RATE_WX = 0x04,
    STATE_ID_RATE_WY = 0x05,
    STATE_ID_RATE_WZ = 0x06,
    STATE_ID_VEL_VX = 0x07,
    STATE_ID_VEL_VY = 0x08,
    STATE_ID_VEL_VZ = 0x09,
    STATE_ID_ALT = 0x0A,
    STATE_ID_COEFF_CL = 0x0B,
    STATE_ID_CANARD_ANGLE = 0x0C,
} can_state_est_id_t;
# 5 "canlib/message/msg_sensor.c" 2

# 1 "canlib/message/msg_common.h" 1
# 26 "canlib/message/msg_common.h"
void write_timestamp_2bytes(uint16_t timestamp, can_msg_t *output);

uint16_t get_message_type(const can_msg_t *msg);

uint8_t get_board_type_unique_id(const can_msg_t *msg);
uint8_t get_board_inst_unique_id(const can_msg_t *msg);

uint16_t get_timestamp(const can_msg_t *msg);
# 6 "canlib/message/msg_sensor.c" 2

# 1 "canlib/message/msg_sensor.h" 1
# 16 "canlib/message/msg_sensor.h"
_Bool build_temp_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint8_t sensor_num, int32_t temp, can_msg_t *output
);




_Bool build_altitude_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, int32_t altitude, can_msg_t *output
);




_Bool build_imu_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t linear_accel,
    uint16_t angular_velocity, can_msg_t *output
);




_Bool build_mag_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t mag_value,
    can_msg_t *output
);






_Bool build_analog_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_analog_sensor_id_t sensor_id, uint16_t sensor_data,
    can_msg_t *output
);

_Bool is_sensor_data(const can_msg_t *msg);





_Bool get_temp_data(const can_msg_t *msg, uint8_t *sensor_num, int32_t *temp);





_Bool get_altitude_data(const can_msg_t *msg, int32_t *altitude);

_Bool get_imu_mag_id_dimension(const can_msg_t *msg, can_imu_id_t *imu_id, char *dimension);





_Bool get_imu_data(const can_msg_t *msg, uint16_t *linear_accel, uint16_t *angular_velocity);

_Bool get_mag_data(const can_msg_t *msg, uint16_t *mag_value);






_Bool get_analog_data(
    const can_msg_t *msg, can_analog_sensor_id_t *sensor_id, uint16_t *output_data
);
# 7 "canlib/message/msg_sensor.c" 2


_Bool build_temp_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, uint8_t sensor_num, int32_t temp, can_msg_t *output
) {
    if (!output) {
        return 0;
    }

    output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_TEMP << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = sensor_num;
    output->data[3] = (temp >> 24) & 0xFF;
    output->data[4] = (temp >> 16) & 0xFF;
    output->data[5] = (temp >> 8) & 0xFF;
    output->data[6] = temp & 0xFF;
    output->data_len = 7;

    return 1;
}

_Bool build_altitude_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, int32_t altitude, can_msg_t *output
) {
    if (!output) {
        return 0;
    }

    output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_ALTITUDE << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (altitude >> 24) & 0xFF;
    output->data[3] = (altitude >> 16) & 0xFF;
    output->data[4] = (altitude >> 8) & 0xFF;
    output->data[5] = altitude & 0xFF;
    output->data_len = 6;

    return 1;
}

_Bool build_imu_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t linear_accel,
    uint16_t angular_velocity, can_msg_t *output
) {
    if (!output) {
        return 0;
    }
    if (axis == 'X') {
        output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_IMU_X << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    } else if (axis == 'Y') {
        output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_IMU_Y << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    } else {
        output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_IMU_Z << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    }

    write_timestamp_2bytes(timestamp, output);

    output->data[2] = imu_id;
    output->data[3] = (linear_accel >> 8) & 0xff;
    output->data[4] = (linear_accel >> 0) & 0xff;
    output->data[5] = (angular_velocity >> 8) & 0xff;
    output->data[6] = (angular_velocity >> 0) & 0xff;
    output->data_len = 7;

    return 1;
}

_Bool build_mag_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, char axis, can_imu_id_t imu_id, uint16_t mag_value,
    can_msg_t *output
) {
    if (!output) {
        return 0;
    }
    if (axis == 'X') {
        output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_MAG_X << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    } else if (axis == 'Y') {
        output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_MAG_Y << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    } else {
        output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_MAG_Z << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    }

    write_timestamp_2bytes(timestamp, output);

    output->data[2] = imu_id;
    output->data[3] = (mag_value >> 8) & 0xff;
    output->data[4] = (mag_value >> 0) & 0xff;
    output->data_len = 5;

    return 1;
}

_Bool build_analog_data_msg(
    can_msg_prio_t prio, uint16_t timestamp, can_analog_sensor_id_t sensor_id, uint16_t sensor_data,
    can_msg_t *output
) {
    if (!output) {
        return 0;
    }

    output->sid = (((uint32_t)prio << 27) | ((uint32_t)MSG_SENSOR_ANALOG << 18) | ((uint32_t)BOARD_TYPE_ID_TELEMETRY << 8) | BOARD_INST_ID_GENERIC);
    write_timestamp_2bytes(timestamp, output);

    output->data[2] = (uint8_t)sensor_id;
    output->data[3] = (sensor_data >> 8) & 0xff;
    output->data[4] = (sensor_data >> 0) & 0xff;

    output->data_len = 5;

    return 1;
}

_Bool is_sensor_data(const can_msg_t *msg) {
    if (!msg) {
        return 0;
    }

    uint16_t type = get_message_type(msg);
    if (type == MSG_SENSOR_TEMP || type == MSG_SENSOR_ALTITUDE || type == MSG_SENSOR_IMU_X ||
        type == MSG_SENSOR_IMU_Y || type == MSG_SENSOR_IMU_Z || type == MSG_SENSOR_MAG_Z ||
        type == MSG_SENSOR_MAG_Y || type == MSG_SENSOR_MAG_Z || type == MSG_SENSOR_ANALOG) {
        return 1;
    } else {
        return 0;
    }
}

_Bool get_temp_data(const can_msg_t *msg, uint8_t *sensor_num, int32_t *temp) {
    if (!msg || !sensor_num || !temp) {
        return 0;
    }
    if (get_message_type(msg) != MSG_SENSOR_TEMP) {
        return 0;
    }

    *sensor_num = msg->data[2];
    *temp = ((uint32_t)msg->data[3] << 24);
    *temp |= ((uint32_t)msg->data[4] << 16);
    *temp |= ((uint32_t)msg->data[5] << 8);
    *temp |= msg->data[6];

    return 1;
}

_Bool get_altitude_data(const can_msg_t *msg, int32_t *altitude) {
    if (!msg || !altitude) {
        return 0;
    }
    if (get_message_type(msg) != MSG_SENSOR_ALTITUDE) {
        return 0;
    }

    *altitude = ((uint32_t)msg->data[3] << 24);
    *altitude |= ((uint32_t)msg->data[4] << 16);
    *altitude |= ((uint32_t)msg->data[5] << 8);
    *altitude |= msg->data[6];

    return 1;
}

_Bool get_imu_mag_id_dimension(const can_msg_t *msg, can_imu_id_t *imu_id, char *dimension) {
    if (!msg || !imu_id || !dimension) {
        return 0;
    }

    can_msg_type_t msg_type = get_message_type(msg);
    if ((msg_type == MSG_SENSOR_IMU_X) || (msg_type == MSG_SENSOR_MAG_X)) {
        *dimension = 'X';
    } else if ((msg_type == MSG_SENSOR_IMU_Y) || (msg_type == MSG_SENSOR_MAG_Y)) {
        *dimension = 'Y';
    } else if ((msg_type == MSG_SENSOR_IMU_Z) || (msg_type == MSG_SENSOR_MAG_Z)) {
        *dimension = 'Z';
    } else {
        return 0;
    }

    *imu_id = msg->data[2];
    return 1;
}

_Bool get_imu_data(const can_msg_t *msg, uint16_t *linear_accel, uint16_t *angular_velocity) {
    if (!msg) {
        return 0;
    }
    if (!linear_accel) {
        return 0;
    }
    if (!angular_velocity) {
        return 0;
    }

    *linear_accel = (uint16_t)msg->data[3] << 8 | msg->data[4];
    *angular_velocity = (uint16_t)msg->data[5] << 8 | msg->data[6];

    return 1;
}

_Bool get_mag_data(const can_msg_t *msg, uint16_t *mag_value) {
    if (!msg) {
        return 0;
    }
    if (!mag_value) {
        return 0;
    }

    *mag_value = (uint16_t)msg->data[3] << 8 | msg->data[4];

    return 1;
}

_Bool get_analog_data(
    const can_msg_t *msg, can_analog_sensor_id_t *sensor_id, uint16_t *output_data
) {
    if (!msg) {
        return 0;
    }
    if (!output_data) {
        return 0;
    }
    if (get_message_type(msg) != MSG_SENSOR_ANALOG) {
        return 0;
    }

    *sensor_id = msg->data[2];
    *output_data = (uint16_t)msg->data[3] << 8 | msg->data[4];

    return 1;
}
