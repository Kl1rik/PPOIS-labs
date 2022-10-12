//
// Created by konstantin on 29.09.22.
//
#include "living.h"

bool clown::size_cell(const std::vector<living*>& cell){
    int counter = 0;
    for(auto & alive: cell){
        if(alive != nullptr){
            counter++;
        }
    }

    if(counter == 4){
        return true;
    }else{
        return false;
    }
}

clown::clown(std::string name, int temp[]) {
    this->name = name;
    this->age = temp[2];
    this->points.first = temp[0];
    this->points.second = temp[1];
    if(temp[3]== 0){
        this->living_sex = male;
    }else{
        this->living_sex = female;
    }
    this->size = temp[4];
    this->hunger = temp[5];

    this->list_of_priority[0] = state::seaweed;
    this->list_of_priority[1] = state::kril;

    this->point_hide.first = -1;
    this->point_hide.second = -1;
}

std::string clown::who() {return "рыба-клоун "+name;}

bool clown::check_die() {
    if(hunger < 10){
        std::cout<<who()<<" died of hunger"<<std::endl;
        return true;
    }else if(age >= 10){
        if( rand()%2 == 0){
            std::cout<<who()<<" just died"<<std::endl;
            return true;
        }else{
            return false;
        }
    }else{
        return  false;
    };
}
std::string clown::getInfo() {
    return "Age: " + std::to_string(this->age) + "\tSize: " + std::to_string(this->size)+
           "\tSex: " + std::to_string(this->living_sex) + "\tHunger: " + std::to_string(this->hunger);
}

state clown::getType() {
    return state::clown;
}

bool clown::hide(const std::vector<std::vector<std::vector<struct living *>>> & field){
    can_hide = false;

    for (auto &alive: field.at(points.first).at(points.second)) {
        if (size_cell(field.at(points.first).at(points.second))){
            break;
        }
        if(alive == nullptr){
            continue;
        }
        if (alive->getType() == state::corals || alive->getType() == state::seaweed) {
            if (alive->getAmouont() > 20) {
                point_hide.first = points.first;
                point_hide.second = points.second;
                can_hide = true;
                std::cout << who() << " hid)"<<std::endl;
                return true;
            }
        }
    }

    if (points.first > 0) {
        for (auto &alive: field.at(points.first - 1).at(points.second)) {
            if (size_cell(field.at(points.first-1).at(points.second))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::corals || alive->getType() == state::seaweed) {
                if (alive->getAmouont() > 20) {
                    point_hide.first = points.first - 1;
                    point_hide.second = points.second;
                    can_hide = true;
                    std::cout << who() << " hid)"<<std::endl;
                    return true;
                }
            }
        }
    }

    if (points.second > 0) {
        for (auto &alive: field.at(points.first).at(points.second - 1)) {
            if (size_cell(field.at(points.first).at(points.second-1))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::corals || alive->getType() == state::seaweed) {
                if (alive->getAmouont() > 20) {
                    point_hide.first = points.first;
                    point_hide.second = points.second - 1;
                    can_hide = true;
                    std::cout << who() << " hid)"<<std::endl;
                    return true;
                }
            }
        }
    }

    if (points.first < field.size() - 1) {
        for (auto &alive: field.at(points.first + 1).at(points.second)) {
            if (size_cell(field.at(points.first+1).at(points.second))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::corals || alive->getType() == state::seaweed) {

                if (alive->getAmouont() > 20) {
                    point_hide.first = points.first+1;
                    point_hide.second = points.second;
                    can_hide = true;
                    std::cout << who() << " hid)"<<std::endl;
                    return true;
                }
            }
        }
    }

    if (points.second < field.at(0).size()-1) {
        for (auto &alive: field.at(points.first).at(points.second + 1)) {
            if (size_cell(field.at(points.first).at(points.second+1))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::corals || alive->getType() == state::seaweed) {
                if (alive->getAmouont() > 20) {
                    point_hide.first = points.first;
                    point_hide.second = points.second+1;
                    can_hide = true;
                    std::cout << who() << " hid)"<<std::endl;
                    return true;
                }
            }
        }
    }

    return can_hide;
}

bool clown::eat(living * who,const std::vector<std::vector<std::vector<struct living *>>> & field) {
    if(!who->getCheckStep()) {
        if (who->hide(field)) {
            return false;
        } else {
            hunger += 10;
            std::cout << who->who() << " was eaten(" << std::endl;
            return true;
        }
    }

    return false;
}

bool clown::propagate() {
    return this->allow_propogate;
}

std::pair<int, int> clown::go(const std::vector<std::vector<std::vector<struct living *>>> & field) {
    std::pair<int,int> point;
    step++;
    check_step = true;


    if(step % 3 == 0){
        allow_propogate = true;
    }
    if (step % 2 == 0) {
        age++;
    }
    if(age == 0){
        return points;
    }
    if(point_hide.first != -1 && can_hide){
        return point_hide;
    }

    if(stop){
        point.first = points.first;
        point.second = points.second;
    }else {
        point = see(field);
        hunger -= 5;
        points.first = point.first;
        points.second = point.second;
    }
    return point;
}

std::pair<int, int> clown::see(const std::vector<std::vector<std::vector<struct living *>>> & field) {
    srand(time(NULL));
    std::pair<int,int> point;

    for (auto &alive: field.at(points.first).at(points.second)) {
        if (size_cell(field.at(points.first).at(points.second))){
            break;
        }
        if(alive == nullptr){
            continue;
        }
        if (alive->getType() == state::clown && alive != this){
            if(alive->getSex() != living_sex && age >= 3 && alive->getAge() >= 3){
                point.first = points.first;
                point.second = points.second;
                if(allow_propogate && alive->propagate()) {
                    alive->setStop();
                    allow_propogate = false;
                    return point;

                }
            }
        }
    }

    if (points.first > 0) {
        for (auto &alive: field.at(points.first - 1).at(points.second)) {
            if (size_cell(field.at(points.first-1).at(points.second))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::clown && alive != this){
                if(alive->getSex() != living_sex && age >= 3 && alive->getAge() >= 3){
                    point.first = points.first - 1;
                    point.second = points.second;
                    if(allow_propogate && alive->propagate()) {
                        alive->setStop();
                        allow_propogate = false;
                        return point;

                    }
                }
            }

        }
    }

    if (points.second > 0) {
        for (auto &alive: field.at(points.first).at(points.second - 1)) {
            if (size_cell(field.at(points.first).at(points.second-1))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::clown && alive != this){
                if(alive->getSex() != living_sex && age >= 3 && alive->getAge() >= 3){
                    point.first = points.first ;
                    point.second = points.second - 1;
                    if(allow_propogate && alive->propagate()) {
                        alive->setStop();
                        allow_propogate = false;
                        return point;

                    }
                }
            }
        }
    }

    if (points.first < field.size() - 1) {
        for (auto &alive: field.at(points.first + 1).at(points.second)) {
            if (size_cell(field.at(points.first+1).at(points.second))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::clown && alive != this){
                if(alive->getSex() != living_sex && age >= 3 && alive->getAge() >= 3){
                    point.first = points.first + 1;
                    point.second = points.second;
                    if(allow_propogate && alive->propagate()) {
                        alive->setStop();
                        allow_propogate = false;
                        return point;

                    }
                }
            }
        }
    }

    if (points.second < field.at(0).size()-1) {
        for (auto &alive: field.at(points.first).at(points.second + 1)) {
            if (size_cell(field.at(points.first).at(points.second+1))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == state::clown && alive != this){
                if(alive->getSex() != living_sex && age >= 3 && alive->getAge() >= 3){
                    point.first = points.first;
                    point.second = points.second + 1;
                    if(allow_propogate && alive->propagate()) {
                        alive->setStop();
                        allow_propogate = false;
                        return point;

                    }
                }
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        for (auto &alive: field.at(points.first).at(points.second)) {
            if (size_cell(field.at(points.first).at(points.second))){
                break;
            }
            if(alive == nullptr){
                continue;
            }
            if (alive->getType() == list_of_priority[i]){
                if(this->size >= alive->getSize()){
                    point.first = points.first;
                    point.second = points.second;
                    if(eat(alive, field)){
                        return point;
                    }
                }
            }
        }

        if (points.first > 0) {
            for (auto &alive: field.at(points.first - 1).at(points.second)) {
                if (size_cell(field.at(points.first-1).at(points.second))){
                    break;
                }
                if(alive == nullptr){
                    continue;
                }
                if (alive->getType() == list_of_priority[i]) {
                    if (this->size >= alive->getSize()) {
                        point.first = points.first - 1;
                        point.second = points.second;
                        if(eat(alive, field)){
                            return point;
                        }
                    }
                }
            }
        }

        if (points.second > 0) {
            for (auto &alive: field.at(points.first).at(points.second - 1)) {
                if (size_cell(field.at(points.first).at(points.second-1))){
                    break;
                }
                if(alive == nullptr){
                    continue;
                }
                if (alive->getType() == list_of_priority[i]) {
                    if (this->size >= alive->getSize()) {
                        point.first = points.first;
                        point.second = points.second - 1;
                        if(eat(alive, field)){
                            return point;
                        }
                    }
                }

            }
        }

        if (points.first < field.size() - 1) {
            for (auto &alive: field.at(points.first + 1).at(points.second)) {
                if (size_cell(field.at(points.first+1).at(points.second))){
                    break;
                }
                if(alive == nullptr){
                    continue;
                }
                if (alive->getType() == list_of_priority[i]) {
                    if (this->size >= alive->getSize()) {
                        point.first = points.first + 1;
                        point.second = points.second;
                        if(eat(alive, field)){
                            return point;
                        }
                    }
                }
            }
        }

        if (points.second < field.at(0).size() - 1) {
            for (auto &alive: field.at(points.first).at(points.second + 1)) {
                if (size_cell(field.at(points.first).at(points.second+1))){
                    break;
                }
                if(alive == nullptr){
                    continue;
                }
                if (alive->getType() == list_of_priority[i]) {
                    if (this->size >= alive->getSize()) {
                        point.first = points.first;
                        point.second = points.second + 1;
                        if(eat(alive, field)){
                            return point;
                        }
                    }
                }
            }
        }
    }


    while(true) {
        int rands = rand() % 5;

        if(rands == 0) {
            if (points.first > 0) {
                for (auto &alive: field.at(points.first - 1).at(points.second)) {
                    if (alive == nullptr) {
                        continue;
                    }
                }
                if (!size_cell(field.at(points.first-1).at(points.second))){
                    point.first = points.first - 1;
                    point.second = points.second;
                    return point;
                }
            }
        }

        if(rands == 1) {
            if (points.second > 0) {
                for (auto &alive: field.at(points.first).at(points.second - 1)) {
                    if (alive == nullptr) {
                        continue;
                    }
                }
                if (!size_cell(field.at(points.first).at(points.second-1))){
                    point.first = points.first ;
                    point.second = points.second - 1;
                    return point;
                }
            }
        }

        if(rands == 2) {
            if (points.first < field.size() - 1) {
                for (auto &alive: field.at(points.first + 1).at(points.second)) {
                    if (alive == nullptr) {
                        continue;
                    }
                }
                if (!size_cell(field.at(points.first+1).at(points.second))) {
                    point.first = points.first + 1;
                    point.second = points.second;
                    return point;
                }
            }
        }

        if(rands == 3) {
            if (points.second < field.at(0).size() - 1) {
                for (auto &alive: field.at(points.first).at(points.second+1)) {
                    if (alive == nullptr) {
                        continue;
                    }
                }
                if (!size_cell(field.at(points.first).at(points.second+1))){
                    point.first = points.first;
                    point.second = points.second + 1;
                    return point;
                }
            }
        }

        if(rands == 4){
            return points;
        }
    }
}

int clown::getSize() {
    return this->size;
}

sex clown::getSex() {
    return this->living_sex;
}
void clown::victim(living *alives) {
    this->alives = alives;
}

living* clown::die_from_other() {
    return nullptr;
}


void clown::setStop() {
    this->stop = true;
    this->propogate = true;
}

std::string clown::getName() {
    return this->name;
}

void clown::setPropogate() {
    this->allow_propogate = false;
    this->propogate = false;
    this->stop = false;
}

std::pair<int, int> clown::getPoint() {
    return points;
}

int clown::getAmouont() {
    return 0;
}

bool clown::getCheckStep() {
    return check_step;
}

void clown::setCheckStep() {
    this->check_step = false;
}

bool clown::getPropogate_state() {
    return this->propogate;
}

int clown::getAge() {
    return this->age;
}

