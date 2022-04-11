#include <stdio.h>
#include <math.h>

// PROGRAM CONSTANTS
// program settings
const char *input_path = "inputaudio1.data";
const char *output_path = "output.data";
const size_t PACKET_LENGTH = 160;

// sample buffer
int8_t buffer[PACKET_LENGTH];

// vad settings
const double MIN_ENERGY = 0.9;
const double INCREASE_FACTOR = 0.0008;
const double FRAME_MARGIN = 5;

// vad algorithm support variables
// TODO struct
size_t n_frame = 0;
double emin, emax;
double delta;
size_t margin_frame_counter;

int vad(int8_t *buffer, size_t packet_length, size_t n_frame, size_t *mf_counter, double *emin, double *emax, double *delta)
{

    // calculate energy of current frame (RMSE)
    double current_energy = 0;
    for (int i = 0; i < packet_length; i++)
    {
        current_energy += pow((double)buffer[i], 2);
    }
    current_energy = sqrt(current_energy / (double)packet_length);

    // if current frame is the first, current energy is emax
    if (n_frame == 0)
    {
        printf("first frame\n");
        *emax = current_energy;
        *emin = MIN_ENERGY;
        *delta = 0;
        *mf_counter = 0;
    }

    if (current_energy > *emax)
        *emax = current_energy;

    // if current energy is less than emin...
    if (current_energy < *emin)
    {
        //... and it is zero
        if (current_energy == 0)
        {
            // set emin back to its default value
            *emin = MIN_ENERGY;
        }
        else
        {
            // otherwise set emin to current energy
            *emin = current_energy;
        }
    }

    // delta = 1;
    printf("\ncurrent energy: %f\n", current_energy);
    // calculate threshold
    double lambda = (*emax - *emin) / (*emax);
    double threshold = (1 - lambda) * (*emax) + lambda * (*emin);
    printf("threshold: %f\n", threshold);
    printf("lambda: %f\n", lambda);
    printf("emin: %f\n", *emin);
    printf("emax: %f\n", *emax);
    printf("mf_count: %d\n", *mf_counter);

    *emin += *emin * (*delta += INCREASE_FACTOR);

    if (current_energy > threshold)
    {
        *mf_counter = 0;
        return 1;
    }

    if (*mf_counter == FRAME_MARGIN)
    {
        return 0;
    }

    *mf_counter = *mf_counter + 1;
    return 1;
}

int main()
{

    FILE *input_file = fopen(input_path, "r");
    FILE *output_file = fopen(output_path, "w");

    int sample; // current sample
    size_t count = 0;
    int vad_val;

    do
    {
        sample = fgetc(input_file);
        buffer[count++] = (int8_t)sample; // current sample is cast to the size of a byte

        if (count == PACKET_LENGTH - 1 || sample == EOF) // if buffer is filled, evaluate the packet
        {
            // printf("packet size: %d\n", count + 1);
            vad_val = vad(buffer, count, n_frame++, &margin_frame_counter, &emin, &emax, &delta);
            for (int i = 0; i < count + 1; i++)
            {
                if (vad_val) // if vad says it's a voice packet...
                {
                    fputc((int)buffer[i], output_file); // ...write the sample to the output stream
                }
                else
                {
                    fputc(0, output_file); // otherwise the packet is noise, write 0
                }
            }
            printf("sent: %d\n", vad_val);
            count = 0;
        }
    } while (sample != EOF);
}
