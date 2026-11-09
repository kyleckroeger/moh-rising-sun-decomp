// sqrt(1 + x) by its binomial series; each term is the previous one times a
// coefficient and x.
float SNDI_rootof1plusx(float x) {
    float t1 = x * 0.5f;
    float t2 = t1 * (x * -0.25f);
    float t3 = t2 * (x * -0.5f);
    float t4 = t3 * (x * -0.625f);
    float t5 = t4 * (x * -0.7f);
    float t6 = t5 * (x * -0.75f);
    float t7 = t6 * (x * -0.7857f);
    return t1 + 1.0f + t2 + t3 + t4 + t5 + t6 + t7;
}
