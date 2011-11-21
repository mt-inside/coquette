int scale_tacho( int in ) /* rpm */
{
    return (int)((float)in * 12.5f);
}

int scale_rpm_ref( int in ) /* rpm */
{
    return (int)((float)in * 8.0f);
}

int scale_maf( int in ) /* mV */
{
    return (int)((float)in * 5.0f);
}

int scale_coolant_temp( int in ) /* degC */
{
    return (int)((float)in - 50.0f);
}
