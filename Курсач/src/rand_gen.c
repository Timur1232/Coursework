const char* const names[] = {
    "Timur Bai",
    "Kirill Kv",
    "Danil Bl",
    "Dmitry K",
    "Boris L",
    "Maks L",
    "Egor Letov",
    "Serj Tankian",
    "Daron Malakian",
    "Chester B",
    "Tom York",
    "Kurt Cobain",
    "Yuri Gagarin",
};

FECNote gen_random_note()
{
    static int count = 1;
    FECNote note;

    note.serialNumber = count++;
    note.factoryNumber = rand() % 100;
    strcpy(note.directorFullName, names[rand() % 13]);
    strcpy(note.engineerFullName, names[rand() % 13]);
    note.energyConsPlan = (float)(rand() % 200 + 50) + (float)(rand() % 100) * 0.01f;
    note.energyConsReal = (float)(rand() % 200 + 50) + (float)(rand() % 100) * 0.01f;

    return note;
}