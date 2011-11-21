#include "scalers.h"

int scale_id( int in )
{
    return in;
}

int scale_tacho( int in )
{
    return (int)((float)in * 12.5f);
}

int scale_rpm_ref( int in )
{
    return (int)((float)in * 8.0f);
}

int scale_maf( int in )
{
    return (int)((float)in * 5.0f);
}

int scale_temp( int in )
{
    return (int)((float)in - 50.0f);
}

int scale_o2( int in )
{
    return (int)((float)in * 10.0f);
}

int scale_road_speed( int in )
{
    return (int)((float)in * 2.0f);
}

int scale_batt_volt( int in )
{
    return (int)((float)in * 80.0f);
}

int scale_tps( int in )
{
    return (int)((float)in * 20.0f);
}

int scale_egt( int in )
{
    return (int)((float)in * 20.0f);
}

int scale_inj_time( int in )
{
    return (int)((float)in / 100.0f);
}

int scale_ign_time( int in )
{
    return (int)(110.0f - (float)in);
}

int scale_aac_valve( int in )
{
    return (int)((float)in / 2.0f);
}
