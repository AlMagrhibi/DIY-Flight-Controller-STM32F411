#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

typedef struct {
    float output;
    float alpha;
} lpf_t;

void lpf_init(lpf_t* filter, float alpha);
float lpf_update(lpf_t* filter, float input);

#endif /* LOWPASS_FILTER_H */
