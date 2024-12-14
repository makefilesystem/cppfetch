// config.use.h
#ifndef CONFIG_USE_H
#define CONFIG_USE_H

/*  Черный: \033[0;30m
    Красный: \033[0;31m
    Зеленый: \033[0;32m
    Желтый: \033[0;33m
    Синий: \033[0;34m
    Фиолетовый: \033[0;35m
    Cyan: \033[0;36m
    Белый: \033[0;37m

    Яркий черный: \033[1;30m
    Яркий красный: \033[1;31m
    Яркий зеленый: \033[1;32m
    Яркий желтый: \033[1;33m
    Яркий синий: \033[1;34m
    Яркий фиолетовый: \033[1;35m
    Яркий cyan: \033[1;36m
    Яркий белый: \033[1;37m
*/

#define COLOR_PRIMARY "\033[1;31m"  
#define COLOR_SECONDARY "\033[0;34m"
#define COLOR_RESET "\033[0m"

/* Пути и настройки расположения данных */
#define ASCII_LOGO_PATH "logo.txt" 
//#define DATA_POS_SIDE
#define DATA_POS_BELOW

/* Включение функциональности */
#define ENABLE_MEMORY
#define ENABLE_CPU
#define ENABLE_OS
#define ENABLE_KERNEL
#define ENABLE_PACKAGES
#define ENABLE_HOSTNAME
#define ENABLE_UPTIME
#define ENABLE_USERNAME
#define ENABLE_IP
#define ENABLE_DISK
#define ENABLE_LOAD
#define ENABLE_DATE_TIME

/* Кастомизация меток */
#define LABEL_MEMORY "Memory: "
#define LABEL_CPU "CPU: "
#define LABEL_OS "OS: "
#define LABEL_KERNEL "Kernel: "
#define LABEL_PACKAGES "Packages: "
#define LABEL_HOSTNAME "Hostname: "
#define LABEL_UPTIME "Uptime: "
#define LABEL_USERNAME "User: "
#define LABEL_IP "IP Address: "
#define LABEL_DISK "Disk Usage: "
#define LABEL_LOAD "CPU Load: "
#define LABEL_DATE_TIME "Date/Time: "

#endif  // CONFIG_USE_H
