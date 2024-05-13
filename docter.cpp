#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

using namespace std;

class Doctor {
private:
    string id;
    string name;

public:
    Doctor(const string& id, const string& name) : id(id), name(name) {}

    string getId() const {
        return id;
    }

    string getName() const {
        return name;
    }
};

class Patient {
private:
    string id;
    string name;

public:
    Patient(const string& id, const string& name) : id(id), name(name) {}

    string getId() const {
        return id;
    }

    string getName() const {
        return name;
    }
};

class ObjectPool {
private:
    map<string, Doctor> doctors;
    map<string, Patient> patients;

public:
    static ObjectPool* getInstance() {
        static ObjectPool instance;
        return &instance;
    }

    map<string, Doctor>& getDoctors() {
        return this->doctors;
    }

    map<string, Patient>& getPatients() {
        return this->patients;
    }

    Doctor* getDoctor(const string& id) {
        if (doctors.find(id) != doctors.end()) {
            return &doctors[id];
        }
        return nullptr;
    }

    Patient* getPatient(const string& id) {
        if (patients.find(id) != patients.end()) {
            return &patients[id];
        }
        return nullptr;
    }

    vector<Doctor*> getDoctorList() {
        vector<Doctor*> doctorList;
        for (auto& pair : doctors) {
            doctorList.push_back(&pair.second);
        }
        return doctorList;
    }

    vector<Patient*> getPatientList() {
        vector<Patient*> patientList;
        for (auto& pair : patients) {
            patientList.push_back(&pair.second);
        }
        return patientList;
    }

    void addDoctor(const Doctor& doctor) {
        doctors[doctor.getId()] = doctor;
    }

    void addPatient(const Patient& patient) {
        patients[patient.getId()] = patient;
    }
};

void loadDataFromFile(const string& filename) {
    ifstream file(filename);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            string type, id, name;
            getline(ss, type, '|');
            getline(ss, id, '|');
            getline(ss, name);

            if (type == "Doctor") {
                ObjectPool::getInstance()->addDoctor(Doctor(id, name));
            } else if (type == "Patient") {
                ObjectPool::getInstance()->addPatient(Patient(id, name));
            }
        }
        file.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
    }
}

int main() {
    loadDataFromFile("data.txt");

    // Retrieving objects from the ObjectPool
    Doctor* retrievedDoctor = ObjectPool::getInstance()->getDoctor("doc001");
    if (retrievedDoctor) {
        cout << "Doctor ID: " << retrievedDoctor->getId() << ", Name: " << retrievedDoctor->getName() << endl;
    }

    Patient* retrievedPatient = ObjectPool::getInstance()->getPatient("pat001");
    if (retrievedPatient) {
        cout << "Patient ID: " << retrievedPatient->getId() << ", Name: " << retrievedPatient->getName() << endl;
    }

    return 0;
}
