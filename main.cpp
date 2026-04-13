#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

const int SIZE = 20;
enum MOUNTAIN_TYPE {UNKNOWN, VOLCANIC, FOLDED, PLATEAU, BLOCK, RESIDUAL, DOME};
const char* type_names[] = {"UNKNOWN","Volcanic","Folded","Plateau","Block","Residual","Dome"};

enum FILTER {
    COUNTRY,
    REGION,
    CONTINENT,
    NAME,
    HEIGHT,
    TYPE
};

struct location {
    string country;
    string region;
    string continent;
};

struct mountain {
    location loc;
    string name;
    int height;
    MOUNTAIN_TYPE type;
};

mountain* init_mountains();
void print_one(const mountain&);
void update_by_height(mountain*, int);
int count_by(const mountain*, FILTER, int);
void print_all(mountain*, int n = SIZE);
bool asc(const mountain&, const mountain&);
bool desc(const mountain&, const mountain&);
string get_field(const mountain&, FILTER);
int count_by(const mountain*, FILTER, const string&);

int filter_by(const mountain*, FILTER, const int);
int filter_by(const mountain*, FILTER, const string);

int filter(const mountain*, mountain*, FILTER, const int);
int filter(const mountain*, mountain*, FILTER, const string);

int filter_by_type(const mountain*, mountain*, const int);
int filter_by_height(const mountain*, mountain*, const int);
int filter_by_name(const mountain*, mountain*, const string);
int filter_by_region(const mountain*, mountain*, const string);
int filter_by_country(const mountain*, mountain*, const string);
int filter_by_continent(const mountain*, mountain*, const string);

void sort_mountains(mountain* target, bool (*cmp)(const mountain&, const mountain&));
void print_sorted(mountain* mount, bool (*cmp)(const mountain&, const mountain&));
void read_from_file(mountain*, string = "mountains.txt");
void save_to_file(mountain*, string = "mount_save.txt");
void print_mountains_by_country(const mountain*);
void capitalize(string&);

int main() {
    mountain* mount = init_mountains();
    mountain* sorted_mount = init_mountains();

    string country = "Switzerland";
    printf("\n+++ Исходная таблица +++\n");
    print_all(mount);
    printf("\n");

    printf("\n+++ Количество вершин в Швейцарии +++\n");
    int n = filter_by_height(mount, sorted_mount, 1000);
    cout << "Количество вершин в " << country << ": " << count_by(mount, COUNTRY, country) << endl;
    printf("\n");

    printf("\n+++ Сортировка +++\n");
    sorted_mount = init_mountains();
    print_sorted(sorted_mount, asc);
    delete[] sorted_mount;
    printf("\n");

    printf("\n+++ Вывод местонахождения гор более 6000 (шеститысячников) +++\n");
    sorted_mount = init_mountains();
    n = filter_by_height(mount, sorted_mount, 6000);
    for (int i = 0; i < n; i++) {cout << get_field(sorted_mount[i], COUNTRY) << endl;}
    delete[] sorted_mount;
    printf("\n");

    printf("\n+++ Замена полей по высоте вершины +++\n");
    sorted_mount = init_mountains();
    update_by_height(sorted_mount, 1500);
    print_all(sorted_mount, 20);
    delete[] sorted_mount;
    printf("\n");

    printf("\n+++ Вывод гор в задланной стране +++\n");
    print_mountains_by_country(mount);
    save_to_file(mount);
    read_from_file(mount);
    print_all(mount);

    delete[] mount;

    return 0;
}

mountain* init_mountains() {
    mountain* mount = new mountain[SIZE];

    mount[0]  = {{"Russia", "Ural", "Europe"}, "Uraltau", 1500, RESIDUAL};
    mount[1]  = {{"Switzerland", "Alps", "Europe"}, "Matterhorn", 1500, VOLCANIC};
    mount[2]  = {{"Switzerland", "Alps", "Europe"}, "MontBlanc", 4810, FOLDED};
    mount[3]  = {{"Switzerland", "Alps", "Europe"}, "Dom", 4545, VOLCANIC};
    mount[4]  = {{"Greece", "Thessaly", "Europe"}, "Olympus", 2917, FOLDED};
    mount[5]  = {{"USA", "New Hampshire", "North America"}, "Washington", 1917, DOME};
    mount[6]  = {{"China", "Tibet", "Asia"}, "Everest", 8848, FOLDED};
    mount[7]  = {{"India", "Himalayas", "Asia"}, "Kangchenjunga", 8586, FOLDED};
    mount[8]  = {{"Tanzania", "Kilimanjaro", "Africa"}, "Kilimanjaro", 6895, VOLCANIC};
    mount[9]  = {{"Japan", "Honshu", "Asia"}, "Fuji", 3776, VOLCANIC};
    mount[10] = {{"Russia", "Ural", "Europe"}, "Iremel", 1582, BLOCK};
    mount[11] = {{"Russia", "Ural", "Europe"}, "Yamantau", 1640, BLOCK};
    mount[12] = {{"Russia", "Ural", "Europe"}, "Taganay", 1178, BLOCK};
    mount[13] = {{"Russia", "Ural", "Europe"}, "Otkliknoy", 1155, BLOCK};
    mount[14] = {{"Russia", "Ural", "Europe"}, "Moskal", 1048, BLOCK};
    mount[15] = {{"Russia", "Ural", "Europe"}, "Kyrybuzhan", 1252, BLOCK};
    mount[16] = {{"Russia", "Ural", "Europe"}, "Zigalga", 1427, BLOCK};
    mount[17] = {{"Brazil", "Guiana", "South America"}, "Roraima", 2810, PLATEAU};
    mount[18] = {{"Russia", "Ural", "Europe"}, "Monakhi", 231, BLOCK};
    mount[19] = {{"Russia", "Caucasus", "Europe"}, "Bermamyt", 2592, PLATEAU};

    return mount;
}



void print_all(mountain* mount, int n) {
    printf("|%-22s|%-22s|%-22s|%-22s|%-10s|%-12s|\n", "CONTINENT", "COUNTRY", "REGION", "NAME", "HEIGHT", "TYPE");
    for (int i = 0; i < n; i++)
        printf("|%-22s|%-22s|%-22s|%-22s|%-10.0d|%-12s|\n",
            mount[i].loc.continent.c_str(),
            mount[i].loc.country.c_str(),
            mount[i].loc.region.c_str(),
            mount[i].name.c_str(),
            mount[i].height,
            type_names[mount[i].type]
        );
}
void print_one(const mountain& mount) {
        printf("Continent: %s, country: %s, region: %s, name: %s, height: %d, type: %s\n",
            mount.loc.continent.c_str(),
            mount.loc.country.c_str(),
            mount.loc.region.c_str(),
            mount.name.c_str(),
            mount.height,
            type_names[mount.type]
        );
}

int filter_by(const mountain* mount, FILTER filter, const string field) {
    switch (filter) {
        case COUNTRY: return mount->loc.country == field; break;
        case REGION: return mount->loc.region == field; break;
        case CONTINENT: return mount->loc.continent == field; break;
        case NAME: return mount->name == field; break;
        default: return 0; break;
    }
}
int filter_by(const mountain* mount, FILTER filter, const int field) {
    switch (filter) {
        case HEIGHT: return mount->height >= field; break;
        case TYPE: return mount->type == field; break;
        default: return 0; break;
    }
}

int filter(const mountain* mount, mountain* result, FILTER filter, const string field) {
    int k = 0;
    for (int i = 0; i < SIZE; i++)
        if (filter_by(&mount[i], filter, field)) result[k++] = mount[i];
    return k;
}
int filter(const mountain* mount, mountain* result, FILTER filter, const int field) {
    int k = 0;
    for (int i = 0; i < SIZE; i++)
        if (filter_by(&mount[i], filter, field)) result[k++] = mount[i];
    return k;
}

int filter_by_country(const mountain* mount, mountain* result, const string country = "") {return filter(mount, result, COUNTRY, country);}
int filter_by_region(const mountain* mount, mountain* result, const string region = "") {return filter(mount, result, REGION, region);}
int filter_by_continent(const mountain* mount, mountain* result, const string continent = "") {return filter(mount, result, CONTINENT, continent);}
int filter_by_name(const mountain* mount, mountain* result, const string name = "") {return filter(mount, result, NAME, name);}
int filter_by_height(const mountain* mount, mountain* result, const int height = -1) {return filter(mount, result, HEIGHT, height);}
int filter_by_type(const mountain* mount, mountain* result, const int type = UNKNOWN) {return filter(mount, result, TYPE, type);}
void print_sorted(mountain* mount, bool (*type_sort)(const mountain&, const mountain&)) {
    sort_mountains(mount, type_sort);
    print_all(mount, SIZE);
}
void capitalize(string& str) {
    if (str.empty()) return;
    str[0] = toupper(static_cast<unsigned char>(str[0]));
    for (size_t i = 1; i < str.size(); ++i) str[i] = tolower(static_cast<unsigned char>(str[i]));
}
int count_by(const mountain* mount, FILTER filter, const string& field) {
    int count = 0;
    for (int i = 0; i < SIZE; i++)
        if (filter_by(&mount[i], filter, field)) count++;
    return count;
}

int count_by(const mountain* mount, FILTER filter, int field) {
    int count = 0;
    for (int i = 0; i < SIZE; i++)
        if (filter_by(&mount[i], filter, field)) count++;
    return count;
}

bool asc(const mountain& a, const mountain& b) {
    return a.height > b.height;
}
bool desc(const mountain& a, const mountain& b) {
    return a.height < b.height;
}
void sort_mountains(mountain* target, bool (*type_sort)(const mountain&, const mountain&)) {
    for (int i = 0; i < SIZE - 1; i++)
        for (int j = 0; j < SIZE - i - 1; j++)
            if (type_sort(target[j], target[j + 1])) {
                mountain temp = target[j];
                target[j] = target[j + 1];
                target[j + 1] = temp;
            }
}

void update_by_height(mountain* mount, int height) {
    for (int i = 0; i < SIZE; i++) {
        if (mount[i].height == height) {
            cout << "Найдена вершина:" << endl;
            print_one(mount[i]);

            cout << "\nВведите новые данные:" << endl;

            cout << "Название: ";
            cin >> mount[i].name;

            cout << "Страна: ";
            cin >> mount[i].loc.country;

            cout << "Регион: ";
            cin >> mount[i].loc.region;

            cout << "Континент: ";
            cin >> mount[i].loc.continent;

            cout << "Тип: " << endl;
            cout << "0 - UNKNOWN" << endl;
            cout << "1 - Volcanic" << endl;
            cout << "2 - Folded" << endl;
            cout << "3 - Plateau" << endl;
            cout << "4 - Block" << endl;
            cout << "5 - Residual" << endl;
            cout << "6 - Dome" << endl;
            int t;
            cin >> t;
            mount[i].type = (MOUNTAIN_TYPE)t;

            return;
        }
    }
    cout << "Вершина с такой высотой не найдена" << endl;
}

string get_field(const mountain& m, FILTER field) {
    switch (field) {
        case COUNTRY: return m.loc.country;
        case REGION: return m.loc.region;
        case CONTINENT: return m.loc.continent;
        case NAME: return m.name;
        case TYPE: return type_names[m.type];
        default: return "NULL";
    }
}

void print_mountains_by_country(const mountain* mount) {
    mountain* sorted_mount = init_mountains();
    string input;
    
    cout << "Введите страну: ";
    cin >> input;
    capitalize(input);
    int n = filter_by_country(mount, sorted_mount, input);
    
    if (n == 0) cout << "\nГоры в " << input << " не найдены" << endl;
    else {
        cout << "\nВершины в " << input << ": " << endl;
        string filtered[SIZE];
        for (int i = 0; i < n; i++) {
            filtered[i] = get_field(sorted_mount[i], NAME);
            cout << filtered[i] << endl;
        }
    }
    
    delete[] sorted_mount;
}

void read_from_file(mountain* mount, string file) {
    ifstream fin;
    string str;
    fin.open(file);

    if (fin.is_open()) {
        string line;

        while (!fin.eof()) {
            getline(fin, line);
            if (line.empty()) continue;

            istringstream iss(line);
            string name;
            int height;

            if (iss >> name >> height) {
                bool found = false;

                for (int i = 0; i < SIZE; i++) 
                    if (mount[i].name == name) {
                        int old_height = mount[i].height;
                        mount[i].height = height;
                        found = true;
                        cout << "Обновлено: " << name << " " << old_height << " = " << height << endl;
                        break;
                    }
                if (!found) cout << "Гора '" << name << "' не найдена" << endl;
            }
        }
    }
    else cout << "Файл не найден" << endl;
    
    fin.close();
}

void save_to_file(mountain* mount, string file) {
    ofstream fout;
    fout.open(file);
    if (fout.is_open()) 
        for (int i = 0; i < SIZE; i++) 
            fout << mount[i].loc.continent << "\t"
                 << mount[i].loc.country   << "\t"
                 << mount[i].loc.region    << "\t"
                 << mount[i].name          << "\t"
                 << mount[i].height        << "\t"
                 << mount[i].type          << endl;
    else cout << "Файл не найден" << endl;
}