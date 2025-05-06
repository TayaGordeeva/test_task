#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

void count_price_and_time(string t, string t_new, string *tab_time, int *price, int price_h) {
    int h_t = stoi(t_new.substr(0, 2)) - stoi(t.substr(0, 2));
    int m_t = stoi(t_new.substr(3, 2)) - stoi(t.substr(3, 2));
    if (m_t < 0) {
        h_t -= 1;
        m_t += 60;
    }
    int h_tmp = stoi((*tab_time).substr(0, 2));
    int m_tmp = stoi((*tab_time).substr(3, 2));
    h_tmp += h_t;
    m_tmp += m_t;

    if (m_tmp >= 60) {
        h_tmp += 1;
        m_tmp -= 60;
    }

    ostringstream oss;
    oss << setw(2) << setfill('0') << h_tmp;
    *tab_time = oss.str() + ":";

    ostringstream oss1;
    oss1 << setw(2) << setfill('0') << m_tmp;
    *tab_time += oss1.str();

    if (m_t > 0) {
        h_t += 1;
    }
    *price += h_t * price_h;
}

int correct_file(int argc) {
    if (argc < 2) {
        cerr << "Error in input string: not file\n";
        return 1;
    }
    return 0;
}

int correct_open_file(ifstream &file) {
    if (file.is_open() == 0) {
        cerr << "Error in input string: not open file\n";
        return 1;
    }
    return 0;
}

int run_task(const std::string &filename) {
    string output = "";

    ifstream file(filename);
    if (correct_open_file(file)) {
        return correct_open_file(file);
    }

    string line1, line2, line3, line;

    getline(file, line1); //table
    const int table = stoi(line1);
    double t1 = stod(line1);
    if ((table <= 0) or (table != t1)) {
        cerr << "Error in 1 string: wrong count of tables\n";
        return 1;
    }

    getline(file, line2);
    string hour_s1 = line2.substr(0, 2); //time of beginning
    int hours_b = stoi(hour_s1);
    string min_s1 = line2.substr(3, 2);
    int min_b = stoi(min_s1);

    string end_time = line2.substr(6, 5);
    string hour_s2 = line2.substr(6, 2); //time of end
    int hours_e = stoi(hour_s2);
    string min_s2 = line2.substr(9, 2);
    int min_e = stoi(min_s2);

    if (((0 > hours_b) or (hours_b > 23) or (0 > min_b) or (min_b > 60) or (0 > hours_e) or (hours_e > 23) or (
             0 > min_e) or (min_e > 60)) or (line2[2] != ':') or (line2[5] != ' ') or (line2[8] != ':')) {
        cerr << "Error in 2 string: wrong time\n";
        return 1;
    }

    getline(file, line3); //price per hour
    int price_h = stoi(line3);
    double p1 = stod(line3);
    if ((price_h <= 0) or (price_h != p1)) {
        cerr << "Error in 3 string: wrong price per hour\n";
        return 1;
    }

    output = output + line2.substr(0, 5) + "\n";

    vector<string> time_all; //vector of client's time
    vector<string> clients; //vector of clients
    vector<string> tab(table); //array of table's busy (clients)
    vector<string> que; //vector of client's queue
    vector<string> tab_time(table); //array of table time
    vector<int> tab_price(table); //array of table price
    vector<string> time_tmp(table);


    for (int i = 0; i < table; i++) {
        tab[i] = " ";
        tab_time[i] = "00:00";
        tab_price[i] = 0;
        time_tmp[i] = "00:00";
    }

    int k = 0;

    while (getline(file, line)) {
        k += 1;
        output = output + line + "\n";
        string time = line.substr(0, 5);
        int h = stoi(time.substr(0, 2));
        int m = stoi(time.substr(3, 2));
        string client;

        if (time_all.size() != 0) {
            if ((stoi((time_all.back()).substr(0, 2)) > h) or (
                    (stoi((time_all.back()).substr(0, 2)) == h) and (stoi((time_all.back()).substr(3, 2)) > m)) or !(
                    (0 <= h) and (h <= 23)) or !((0 <= m) and (m <= 59))) {
                cerr << "Error in " << k + 3 << " string: wrong time\n";
                return 1;
            }
        }

        if (!((0 <= h) and (h <= 23)) or !((0 <= m) and (m <= 59))) {
            cerr << "Error in " << k + 3 << " string: wrong time\n";
            return 1;
        }
        time_all.push_back(time);

        int id = stoi(line.substr(6, 1)); //id input
        switch (id) {
            case 1: {
                client = line.substr(8, line.length() - 8);

                if (find(clients.begin(), clients.end(), client) != clients.end()) {
                    output = output + time + " 13 YouShallNotPass\n";
                } else if ((h < hours_b) or ((h == hours_b) and (m < min_b))) {
                    output = output + time + " 13 NotOpenYet\n";
                } else {
                    clients.push_back(client);
                }
                break;
            }

            case 2: {
                string inf = line.substr(8, line.length() - 8);
                istringstream iss(inf);
                string t_str;
                getline(iss, client, ' '); //client
                getline(iss, t_str);
                int t = stoi(t_str) - 1; //number of table

                auto ind = find(clients.begin(), clients.end(), client);
                if (ind == clients.end()) {
                    output = output + time + " 13 ClientUnknown\n";
                } else if (tab[t] != " ") {
                    output = output + time + " 13 PlaceIsBusy\n";
                } else {
                    for (int i = 0; i < table; i++) {
                        if (tab[i] == client) {
                            tab[i] = " ";
                            count_price_and_time(time_tmp[i], time, &tab_time[i], &tab_price[i], price_h);
                            time_tmp[i] = "00:00";
                        }
                    }
                    tab[t] = client;
                    time_tmp[t] = time;
                }
                break;
            }

            case 3: {
                client = line.substr(8, line.length() - 8);

                if (find(tab.begin(), tab.end(), " ") != tab.end()) {
                    output = output + time + " 13 ICanWaitNoLonger!\n";
                } else if (que.size() >= table) {
                    output = output + time + " 11 " + client + "\n";
                } else {
                    if (find(que.begin(), que.end(), client) == que.end()) {
                        que.push_back(client);
                    }
                }
                break;
            }

            case 4: {
                client = line.substr(8, line.length() - 8);
                auto ind = find(clients.begin(), clients.end(), client);

                if (ind == clients.end()) {
                    output = output + time + " 13 ClientUnknown\n";
                } else {
                    clients.erase(ind);
                    for (int i = 0; i < table; i++) {
                        if (tab[i] == client) {
                            tab[i] = " ";
                            count_price_and_time(time_tmp[i], time, &tab_time[i], &tab_price[i], price_h);
                            time_tmp[i] = "00:00";
                            if (!que.empty()) {
                                tab[i] = que.front();
                                client = que.front();
                                time_tmp[i] = time;
                                que.erase(que.begin());
                                output = output + time + " 12 " + client + " " + to_string(i + 1) + "\n";
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    cout << output;

    vector<string> end_cl; // all clients at the end of the day
    if (!que.empty()) {
        for (int i = 0; i < que.size(); i++) {
            end_cl.push_back(que[i]);
        }
    }

    if (!clients.empty()) {
        for (int i = 0; i < clients.size(); i++) {
            end_cl.push_back(clients[i]);
        }
    }

    sort(end_cl.end(), end_cl.end());

    for (int i = 0; i < end_cl.size(); i++) {
        cout << end_time << " 11 " << end_cl[i] << endl;
    }

    cout << end_time << endl;

    for (int i = 0; i < table; i++) {
        if (time_tmp[i] != "00:00") {
            int h_end = stoi((end_time).substr(0, 2));
            int m_end = stoi((end_time).substr(3, 2));
            int h_i = stoi((time_tmp[i]).substr(0, 2));
            int m_i = stoi((time_tmp[i]).substr(3, 2));

            int h = h_end - h_i;
            int m = m_end - m_i;
            if (m < 0) {
                h -= 1;
                m += 60;
            }
            ostringstream oss;
            oss << setw(2) << setfill('0') << h;
            tab_time[i] = oss.str() + ":";

            ostringstream oss1;
            oss1 << setw(2) << setfill('0') << m;
            tab_time[i] += oss1.str();

            if (m > 0) {
                h += 1;
            }
            tab_price[i] += h * price_h;
        }
    }

    for (int i = 0; i < table; i++) {
        cout << i + 1 << " " << tab_price[i] << " " << tab_time[i] << endl;
    }

    return 0;
}


#ifndef TESTING
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error in input string: not file\n";
        return 1;
    }
    return run_task(argv[1]);
}
#endif
