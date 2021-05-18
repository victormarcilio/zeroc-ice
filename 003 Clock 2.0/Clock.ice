module Demo
{
    struct Horario
    {
        int hours;
        int minutes;
        int seconds;
    }
    interface Clock
    {
        Horario getTime();
        void setTime(Horario h);
    }
}
