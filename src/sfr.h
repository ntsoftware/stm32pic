#pragma once

struct sfr {
    void (*init)(void);
    uint8_t (*rd)(void);
    void (*wr)(uint8_t value);
};

extern const struct sfr sfr_fsr;
extern const struct sfr sfr_gpio;
extern const struct sfr sfr_indf;
extern const struct sfr sfr_option;
extern const struct sfr sfr_osccal;
extern const struct sfr sfr_pcl;
extern const struct sfr sfr_status;
extern const struct sfr sfr_tmr0;
extern const struct sfr sfr_trisgpio;
