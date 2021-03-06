// Copyright 2019 ARAOvsepyan
#include "../include/json.hpp"

Json::Json() {}

Json::Json(const std::vector<std::any>& v) {
    vec = v;
}

Json::Json(const std::string & s) {
    int i = 0;
    std::string key = "", value1 = "";
    std::any value;
    while (s[i] != '\0') {
        if (s[i] == '"') {
            ++i;
            while (s[i] != '"') {
                key += s[i];
                ++i;
            }
        }
        if (s[i] == ':') {
            ++i;
            while (s[i] == ' ') {
                ++i;
            }
            if (s[i] == '"') {
                ++i;
                while (s[i] != '"') {
                    value1 += s[i];
                    ++i;
                }
                object.insert(std::pair<std::string,
                        std::string>(key, value1));
                key = value1 = "";
            } else {
                if (s[i] == '[') {
                    std::vector<std::any> v;
                    while (s[i] != ']') {
                        ++i;
                        while (s[i] == ' ') {
                            ++i;
                        }
                        if (isdigit(s[i])) {
                            v.push_back(atof(&(s[i])));
                            while (s[i] != ',' && s[i] != ']') {
                                ++i;
                            }
                        } else {
                            if (s[i] == '"') {
                                ++i;
                                std::string s_arr = "";
                                while (s[i] != '"') {
                                    s_arr += s[i];
                                    ++i;
                                }
                                v.push_back(s_arr);
                            } else {
                                if (s[i] == '[') {
                                    ++i;
                                    std::string s_mas = "";
                                    while (s[i] != ']'){
                                        s_mas += s[i];
                                        ++i;
                                    }
                                    s_mas += " ";
                                    ++i;
                                    v.push_back(Json(s_mas));
                                } else {
                                    if (s[i] == '{') {
                                        ++i;
                                        std::string str = "";
                                        while (s[i] != '}'){
                                            str += s[i];
                                            ++i;
                                        }
                                        v.push_back(Json(str));
                                    }
                                }
                            }
                        }
                    }
                    Json temp_arr(v);
                    value = temp_arr;
                    object.insert(std::pair<std::string,
                            std::any>(key, value));
                } else {
                    if (s[i] == '{') {
                        std::string temp_s;
                        while (s[i] != '}') {
                            temp_s += s[i];
                            ++i;
                        }
                        temp_s += '}';
                        Json temp(temp_s);
                        object.insert(
                                std::pair<std::string, Json>(key, temp));
                    } else {
                        if (isdigit(s[i])) {
                            value = atof(&(s[i]));
                            object.insert(
                                    std::pair<std::string,
                                std::any>(key, value));
                        } else {
                            if (s[i] == 'f') {
                                std::string s_bool = "";
                                while (s[i] != ',' && s[i] != '"'
                                       && s[i] != ' ') {
                                    s_bool += s[i];
                                    ++i;
                                }
                                if (s_bool == "false") {
                                    value = false;
                                    object.insert(
                                            std::pair<std::string,
                                        std::any>(key, value));
                                }
                            } else {
                                if (s[i] == 't') {
                                    std::string s_bool = "";
                                    while (s[i] != ',') {
                                        s_bool += s[i];
                                        ++i;
                                    }
                                    if (s_bool == "true") {
                                        value = true;
                                        object.insert(
                                                std::pair<std::string,
                                            std::any>(key, value));
                                    }
                                } else {
                                    if (s[i] == 'n') {
                                        std::string s_bool = "";
                                        while (s[i] != ',' && s[i] != ' '
                                               && s[i] != '"') {
                                            s_bool += s[i];
                                            ++i;
                                        }
                                        if (s_bool == "null") {
                                            value = NULL;
                                            object.insert(
                                                    std::pair<std::string,
                                                std::any>(key, value));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                key = "";
            }
        }
        ++i;
    }
}

bool Json::is_array() const {
    return !vec.empty();
}

bool Json::is_object() const{
    return !object.empty();
}

std::any & Json::operator[](const std::string & key) {
    if (this->is_object()) {
        for (auto it = object.begin();
             it != object.end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }
    }
    throw std::runtime_error("it`s not an object");
}

std::any & Json::operator[](size_t index) {
    if (this->is_array()) {
        if (index < vec.size()) {
            return this->vec[index];
        }
    }
    throw std::runtime_error("it`s not an array");
}

Json Json::parse(const std::string & s) {
    return Json(s);
}

Json Json::parseFile(const std::string & path_to_file) {
    std::ifstream fin(path_to_file);
    char temp;
    std::string s;
    if (fin.is_open()) {
        while (!fin.eof()) {
            fin >> temp;
            s += temp;
        }
    } else {
        std::cout << "Данного файла не существует." << std::endl;
    }
    fin.close();
    return Json(s);
}
