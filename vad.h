#include <stdio.h>
#include <math.h>

int vad(int8_t *buffer, size_t packet_length, size_t n_frame, size_t *mf_counter, double *emin, double *emax, double *delta);

struct vad_settings {

};