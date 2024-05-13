#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::map;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

#define ENABLE_NEW_ALLOC
#define ENABLE_DE_ALLOC

class ObjectCounter
{
    map<string, int> counter;
    static ObjectCounter *instance;

    ObjectCounter() {}

public:
    static ObjectCounter *getInstance()
    {
        if (instance == NULL)
        {
            instance = new ObjectCounter;
        }
        return instance;
    }

    void increment(string className)
    {
        int countObject = counter[className];
        counter[className] = ++countObject;
    }

    void decrement(string className)
    {
        int countObject = counter[className];
        counter[className] = --countObject;
    }

    map<string, int> getCounter()
    {
        return counter;
    }
};

ObjectCounter *ObjectCounter::instance;

#ifdef ENABLE_DE_ALLOC

inline void new_alloc(string className)
{
    ObjectCounter::getInstance()->increment(className);
}

#endif

#ifdef ENABLE_DE_ALLOC

inline void de_alloc(string className)
{
    ObjectCounter::getInstance()->decrement(className);
}

#endif

// common base class for all the entity class
class DataObject
{
public:
    virtual string toString() const = 0;
    virtual ~DataObject() {}
};

class Person : public DataObject
{
protected:
    string name;
    string dateOfBirth;
    string gender;
    string mobile;
    string city;

public:
    Person() {}

    Person(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_)
        : name(name_), dateOfBirth(dateOfBirth_), gender(gender_), mobile(mobile_), city(city_) {}

    string getName() const { return name; }
    void setName(const string &name) { this->name = name; }

    string getDateOfBirth() const { return dateOfBirth; }
    void setDateOfBirth(const string &dateOfBirth) { this->dateOfBirth = dateOfBirth; }

    string getGender() const { return gender; }
    void setGender(const string &gender) { this->gender = gender; }

    string getMobile() const { return mobile; }
    void setMobile(const string &mobile) { this->mobile = mobile; }

    string getCity() const { return city; }
    void setCity(const string &city) { this->city = city; }

    // utility function
    virtual string toString() const
    {
        stringstream ss;
        ss << "Name: " << name << "\n";
        ss << "Date of Birth: " << dateOfBirth << "\n";
        ss << "Gender: " << gender << "\n";
        ss << "Mobile: " << mobile << "\n";
        ss << "City: " << city << "\n";

        return ss.str();
    }
};

class MedicalRecord;

class Patient : public Person
{
private:
    string patientID;
    vector<MedicalRecord> medicalRecords;

public:
    Patient() : Person()
    {
        new_alloc("Patient");
    }

    Patient(const string &patientID_, const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_)
        : Person(name_, dateOfBirth_, gender_, mobile_, city_), patientID(patientID_)
    {
        new_alloc("Patient");
    }

    ~Patient()
    {
        de_alloc("Patient");
    }

    string getPatientID() const { return patientID; }
    void setPatientID(const string &patientID) { this->patientID = patientID; }

    const vector<MedicalRecord> &getMedicalRecords() const { return medicalRecords; }
    void addMedicalRecord(const MedicalRecord &medicalRecord) { medicalRecords.push_back(medicalRecord); }

    virtual string toString() const;
};

class Staff : public Person
{
protected:
    string id;

public:
    Staff() : Person() {}

    Staff(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_)
        : Person(name_, dateOfBirth_, gender_, mobile_, city_), id(id_) {}

    string getId() const { return id; }
    void setId(const string &id) { this->id = id; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "ID: " << id << "\n";
        ss << Person::toString();
        return ss.str();
    }
};

class AdministrativeStaff : public Staff
{
private:
    string designation;

public:
    AdministrativeStaff() : Staff() {}

    AdministrativeStaff(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_, const string &designation_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), designation(designation_) {}

    string getDesignation() const { return designation; }
    void setDesignation(const string &designation) { this->designation = designation; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Designation: " << designation << "\n";
        ss << Staff::toString();
        return ss.str();
    }
};

class Doctor;

class MedicalRecord : public DataObject
{
private:
    string recordID;
    Patient *patient;
    Doctor *doctor;
    string diagnosis;
    string treatment;
    string medications;
    string labResults;

public:
    MedicalRecord() : patient(nullptr), doctor(nullptr) {}

    MedicalRecord(const string &recordID_, Patient *patient_, Doctor *doctor_, const string &diagnosis_, const string &treatment_, const string &medications_, const string &labResults_)
        : recordID(recordID_), patient(patient_), doctor(doctor_), diagnosis(diagnosis_), treatment(treatment_), medications(medications_), labResults(labResults_) {}

    // Getters and setters
    string getRecordID() const { return recordID; }
    void setRecordID(const string &recordID) { this->recordID = recordID; }

    Patient *getPatient() const { return patient; }
    void setPatient(Patient *patient) { this->patient = patient; }

    Doctor *getDoctor() const { return doctor; }
    void setDoctor(Doctor *doctor) { this->doctor = doctor; }

    string getDiagnosis() const { return diagnosis; }
    void setDiagnosis(const string &diagnosis) { this->diagnosis = diagnosis; }

    string getTreatment() const { return treatment; }
    void setTreatment(const string &treatment) { this->treatment = treatment; }

    string getMedications() const { return medications; }
    void setMedications(const string &medications) { this->medications = medications; }

    string getLabResults() const { return labResults; }
    void setLabResults(const string &labResults) { this->labResults = labResults; }

    string toString() const
    {
        return "Medical Record ID: " + recordID + ", Diagnosis: " + diagnosis + ", Treatment: " + treatment;
    }
};

string Patient::toString() const
{
    stringstream ss;
    ss << "Patient ID: " << patientID << "\n";
    ss << Person::toString();
    for (const auto &record : medicalRecords)
    {
        ss << "Medical Record: " << record.toString() << "\n";
    }
    return ss.str();
}

class ShiftDetails : public DataObject
{
private:
    string shiftType;
    string timeSlotRange;

public:
    ShiftDetails() {}

    ShiftDetails(const string &shiftType_, const string &timeSlotRange_)
        : shiftType(shiftType_), timeSlotRange(timeSlotRange_) {}

    // Getters and setters
    string getShiftType() const { return shiftType; }
    void setShiftType(const string &shiftType) { this->shiftType = shiftType; }

    string getTimeSlotRange() const { return timeSlotRange; }
    void setTimeSlotRange(const string &timeSlotRange) { this->timeSlotRange = timeSlotRange; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Shift Type: " << shiftType << "\n";
        ss << "Time Slot Range: " << timeSlotRange << "\n";
        return ss.str();
    }
};

class ShiftSchedule : public DataObject
{
private:
    map<string, ShiftDetails> shifts; // Maps month to shift details

public:
    ShiftSchedule() {}

    void addShift(const string &month, const ShiftDetails &shiftDetails)
    {
        shifts[month] = shiftDetails;
    }

    ShiftDetails getShift(const string &month) const
    {
        auto it = shifts.find(month);
        if (it != shifts.end())
        {
            return it->second;
        }
        return ShiftDetails("", "");
    }

    virtual string toString() const
    {
        stringstream ss;
        for (const auto &entry : shifts)
        {
            ss << "Month: " << entry.first << "\n";
            ss << entry.second.toString() << "\n"; // Use the toString() method of ShiftDetails
        }
        return ss.str();
    }
};

class Doctor : public Staff
{
private:
    string specialization;
    ShiftSchedule shiftSchedule;

public:
    Doctor() : Staff(), shiftSchedule()
    {
        new_alloc("Doctor");
    }

    Doctor(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_, const string &specialization_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), specialization(specialization_), shiftSchedule()
    {
        new_alloc("Doctor");
    }

    ~Doctor()
    {
        de_alloc("Doctor");
    }
    string getSpecialization() const { return specialization; }
    void setSpecialization(const string &specialization) { this->specialization = specialization; }

    const ShiftSchedule &getShiftSchedule() const { return shiftSchedule; }
    void setShiftSchedule(const ShiftSchedule &schedule) { shiftSchedule = schedule; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Specialization: " << specialization << "\n";
        ss << shiftSchedule.toString(); // Use the toString() method of ShiftSchedule
        ss << Staff::toString();
        return ss.str();
    }
};

class Nurse : public Staff
{
private:
    string specialization;
    ShiftSchedule shiftSchedule;

public:
    Nurse() : Staff(), shiftSchedule() {}

    Nurse(const string &name_, const string &dateOfBirth_, const string &gender_, const string &mobile_, const string &city_, const string &id_, const string &specialization_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), specialization(specialization_), shiftSchedule() {}

    string getSpecialization() const { return specialization; }
    void setSpecialization(const string &specialization) { this->specialization = specialization; }

    const ShiftSchedule &getShiftSchedule() const { return shiftSchedule; }
    void setShiftSchedule(const ShiftSchedule &schedule) { shiftSchedule = schedule; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Specialization: " << specialization << "\n";
        ss << shiftSchedule.toString();
        ss << Staff::toString();
        return ss.str();
    }
};

class Department : public DataObject
{
private:
    string departmentID;
    string name;
    string description;
    string doctorId;

public:
    Department() 
    {
        new_alloc("Department");
    }

    Department(const string& departmentID_, const string& name_, const string& description_, const string& doctorId_)
        : departmentID(departmentID_), name(name_), description(description_), doctorId(doctorId_) 
    {
        new_alloc("Department");
    }

    ~Department()
    {
        de_alloc("Department");
    }

    // Setter and Getter pairs
    void setDepartmentID(const string& departmentID) { this->departmentID = departmentID; }
    string getDepartmentID() const { return departmentID; }

    void setName(const string& name) { this->name = name; }
    string getName() const { return name; }

    void setDescription(const string& description) { this->description = description; }
    string getDescription() const { return description; }

    void setDoctorId(const string& doctorId) { this->doctorId = doctorId; }
    string getDoctorId() const { return doctorId; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Department ID: " << departmentID << "\n";
        ss << "Name: " << name << "\n";
        ss << "Description: " << description << "\n";
        ss << "Doctor Id: " << doctorId << "\n";
        return ss.str();
    }
};

class Appointment : public DataObject
{
private:
    string appointmentID;
    Patient *patient;
    Person *doctor;
    string dateTime;
    string reason;

public:
    Appointment() : patient(nullptr), doctor(nullptr) 
    {
        new_alloc("Appointment");
    }

    ~Appointment()
    {
        de_alloc("Appointment");
    }

    Appointment(const string &appointmentID_, Patient *patient_, Person *doctor_, const string &dateTime_, const string &reason_)
        : appointmentID(appointmentID_), patient(patient_), doctor(doctor_), dateTime(dateTime_), reason(reason_) 
    {
        new_alloc("Appointment");
    }

    // Setter and Getter pairs
    void setAppointmentID(const string &appointmentID) { this->appointmentID = appointmentID; }
    string getAppointmentID() const { return appointmentID; }

    void setPatient(Patient *patient) { this->patient = patient; }
    Person *getPatient() const { return patient; }

    void setDoctor(Person *doctor) { this->doctor = doctor; }
    Person *getDoctor() const { return doctor; }

    void setDateTime(const string &dateTime) { this->dateTime = dateTime; }
    string getDateTime() const { return dateTime; }

    void setReason(const string &reason) { this->reason = reason; }
    string getReason() const { return reason; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Appointment ID: " << appointmentID << "\n";
        ss << "Patient: " << patient->getName() << "\n";
        ss << "Doctor: " << doctor->getName() << "\n";
        ss << "Date & Time: " << dateTime << "\n";
        ss << "Reason: " << reason << "\n";
        return ss.str();
    }
};

class Billing : public DataObject
{
private:
    string billID;
    Person *patient;
    vector<string> servicesProvided;
    double cost;
    bool paymentStatus;

public:
    Billing() : patient(nullptr), cost(0), paymentStatus(false) 
    {
        new_alloc("Builling");
    }

    ~Billing()
    {
        de_alloc("DepartmentForm");
    }

    Billing(const string &billID_, Person *patient_, const vector<string> &servicesProvided_, double cost_)
        : billID(billID_), patient(patient_), servicesProvided(servicesProvided_), cost(cost_), paymentStatus(false) 
    {
        new_alloc("Builling");
    }

    // Setter and Getter pairs
    void setBillID(const string &billID) { this->billID = billID; }
    string getBillID() const { return billID; }

    void setPatient(Person *patient) { this->patient = patient; }
    Person *getPatient() const { return patient; }

    void setServicesProvided(const vector<string> &servicesProvided) { this->servicesProvided = servicesProvided; }
    const vector<string> &getServicesProvided() const { return servicesProvided; }

    void setCost(double cost) { this->cost = cost; }
    double getCost() const { return cost; }

    void setPaymentStatus(bool status) { this->paymentStatus = status; }
    bool getPaymentStatus() const { return paymentStatus; }

    virtual string toString() const
    {
        stringstream ss;
        ss << "Bill ID: " << billID << "\n";
        ss << "Patient: " << patient->getName() << "\n";
        ss << "Services Provided:\n";
        for (const auto &service : servicesProvided)
        {
            ss << "- " << service << "\n";
        }
        ss << "Cost: $" << cost << "\n";
        ss << "Payment Status: " << (paymentStatus ? "Paid" : "Pending") << "\n";
        return ss.str();
    }
};

class Prescription : public DataObject {
private:
    string prescriptionID;
    string patient;
    string doctor;
    string prescriptionDetails;
    string datePrescribed;

public:
    Prescription() {
        new_alloc("Prescription");//function calls
    }

    Prescription(const string& prescriptionID_, const string& patient_, const string& doctor_, const string& prescriptionDetails_, const string& datePrescribed_)
        : prescriptionID(prescriptionID_), patient(patient_), doctor(doctor_), prescriptionDetails(prescriptionDetails_), datePrescribed(datePrescribed_) {
            new_alloc("Prescription");
    }

    ~Prescription(){
        de_alloc("Prescription");
    }

    string getPrescriptionID() const
     { return prescriptionID; }
    void setPrescriptionID(const string& prescriptionID) 
    { this->prescriptionID = prescriptionID; }

    string getPatient() const 
    { return patient; }
    void setPatient(const string& patient) 
    { this->patient = patient; }

    string getDoctor() const
     { return doctor; }
    void setDoctor(const string& doctor) 
    { this->doctor = doctor; }

    string getPrescriptionDetails() const
     { return prescriptionDetails; }
    void setPrescriptionDetails(const string& prescriptionDetails)
     { this->prescriptionDetails = prescriptionDetails; }

    string getDatePrescribed() const 
    { return datePrescribed; }
    void setDatePrescribed(const string& datePrescribed) 
    { this->datePrescribed = datePrescribed; }

    string toString() const override 
    {
        stringstream ss;
        ss << "Prescription ID: " << prescriptionID <<"\n";
        ss << "Patient: " << patient << "\n";
        ss<<"Doctor: " << doctor<<"\n";
        ss<< "Prescription Details: " << prescriptionDetails<<"\n";
        ss<< "Date Prescribed: " << datePrescribed<<"\n";
        return ss.str();
    }
};

class Item : public DataObject
{
private:
    string itemID;
    string itemName;
    string expiryDate;

public:
    Item()
    {
        new_alloc("Item");
    }

    ~Item()
    {
        de_alloc("Item");
    }

    Item(const string &itemID_, const string &itemName_, const string &expiryDate_)
        : itemID(itemID_), itemName(itemName_), expiryDate(expiryDate_) 
    {
        new_alloc("Item");
    }

    string getItemID() const { return itemID; }
    void setItemID(const string &itemID) { this->itemID = itemID; }

    string getItemName() const { return itemName; }
    void setItemName(const string &itemName) { this->itemName = itemName; }

    string getExpiryDate() const { return expiryDate; }
    void setExpiryDate(const string &expiryDate) { this->expiryDate = expiryDate; }

    string toString() const
    {
        return "Item ID: " + itemID + ", Item Name: " + itemName + ", Expiry Date: " + expiryDate;
    }
};

class Supplier : public DataObject
{
private:
    string supplierID;
    string name;
    string mobile;

public:
    Supplier()
    {
        new_alloc("Supplier");
    }

    ~Supplier()
    {
        de_alloc("Supplier");
    }

    Supplier(const string &supplierID_, const string &name_, const string &mobile_)
        : supplierID(supplierID_), name(name_), mobile(mobile_) 
    {
        new_alloc("Supplier");
    }

    string getSupplierID() const { return supplierID; }
    void setSupplierID(const string &supplierID) { this->supplierID = supplierID; }

    string getName() const { return name; }
    void setName(const string &name) { this->name = name; }

    string getMobile() const { return mobile; }
    void setMobile(const string &mobile) { this->mobile = mobile; }

    string toString() const
    {
        return "Supplier ID: " + supplierID + ", Name: " + name + ", Mobile: " + mobile;
    }
};

class StockedItem : public DataObject
{
private:
    string dtPurchase;
    int quantity;
    Item *item;
    Supplier *supplier;

public:
    StockedItem()
    {
        new_alloc("StockedItem");
    }

    ~StockedItem()
    {
        de_alloc("StockedItem");
    }

    StockedItem(const string &dtPurchase_, int quantity_, Item *item_, Supplier *supplier_)
        : dtPurchase(dtPurchase_), quantity(quantity_), item(item_), supplier(supplier_) 
    {
        new_alloc("StockedItem");
    }

    string getDtPurchase() const { return dtPurchase; }
    void setDtPurchase(const string &dtPurchase) { this->dtPurchase = dtPurchase; }

    int getQuantity() const { return quantity; }
    void setQuantity(int quantity) { this->quantity = quantity; }

    Item *getItem() const { return item; }
    void setItem(Item *item) { this->item = item; }

    Supplier *getSupplier() const { return supplier; }
    void setSupplier(Supplier *supplier) { this->supplier = supplier; }

    string toString() const
    {
        return "Date of Purchase: " + dtPurchase + ", Quantity: " + std::to_string(quantity);
    }
};

class Inventory
{
private:
    vector<StockedItem> stockedItems;

public:
    Inventory()
    {
        new_alloc("Inventory");
    }

    ~Inventory()
    {
        de_alloc("Inventory");
    }

    void addStockedItem(const StockedItem &stockedItem)
    {
        stockedItems.push_back(stockedItem);
    }

    const vector<StockedItem> &getStockedItems() const
    {
        return stockedItems;
    }
};

class Test : public DataObject
{
private:
    string testID;
    Patient *patient;
    AdministrativeStaff *technician;
    string testType;
    string result;

public:
    Test() : patient(nullptr), technician(nullptr) 
    {
        new_alloc("Test");
    }

    Test(const string &testID_, Patient *patient_, AdministrativeStaff *technician_, const string &testType_, const string &result_)
        : testID(testID_), patient(patient_), technician(technician_), testType(testType_), result(result_) 
    {
        new_alloc("Test");
    }

    string getTestID() const { return testID; }
    void setTestID(const string &testID) { this->testID = testID; }

    Patient *getPatient() const { return patient; }
    void setPatient(Patient *patient) { this->patient = patient; }

    AdministrativeStaff *getTechnician() const { return technician; }
    void setTechnician(AdministrativeStaff *technician) { this->technician = technician; }

    string getTestType() const { return testType; }
    void setTestType(const string &testType) { this->testType = testType; }

    string getResult() const { return result; }
    void setResult(const string &result) { this->result = result; }

    string toString() const
    {
        return "Test ID: " + testID + ", Test Type: " + testType + ", Result: " + result;
    }

    ~Test()
    {
        de_alloc("Test");
    }
};

class Lab : public DataObject
{
private:
    vector<Test> tests;

public:
    Lab()
    {
        new_alloc("Lab");
    }

    const vector<Test> &getTests() const { return tests; }

    void addTest(const Test &test)
    {
        tests.push_back(test);
    }

    string toString() const
    {
        string output = "Lab Tests:\n";
        for (const auto &test : tests)
        {
            output += test.toString() + "\n";
        }
        return output;
    }

    ~Lab()
    {
        de_alloc("Lab");
    }
};

class Hospital : public DataObject
{
private:
    vector<Department> departments;
    vector<Doctor> doctors;
    vector<Nurse> nurses;
    vector<AdministrativeStaff> administrativeStaff;
    vector<Patient> patients;
    vector<Appointment> appointments;
    vector<MedicalRecord> medicalRecords;
    vector<Billing> bills;
    Lab lab;
    Inventory inventory;

public:
    // Constructors
    Hospital() 
    {
        new_alloc("Hospital");
    }

    // Getter and setter for departments
    const vector<Department> &getDepartments() const { return departments; }
    void addDepartment(const Department &department) { departments.push_back(department); }

    // Getter and setter for doctors
    const vector<Doctor> &getDoctors() const { return doctors; }
    void addDoctor(const Doctor &doctor) { doctors.push_back(doctor); }

    // Getter and setter for nurses
    const vector<Nurse> &getNurses() const { return nurses; }
    void addNurse(const Nurse &nurse) { nurses.push_back(nurse); }

    // Getter and setter for administrative staff
    const vector<AdministrativeStaff> &getAdministrativeStaff() const { return administrativeStaff; }
    void addAdministrativeStaff(const AdministrativeStaff &staff) { administrativeStaff.push_back(staff); }

    // Getter and setter for patients
    const vector<Patient> &getPatients() const { return patients; }
    void addPatient(const Patient &patient) { patients.push_back(patient); }

    // Getter and setter for appointments
    const vector<Appointment> &getAppointments() const { return appointments; }
    void addAppointment(const Appointment &appointment) { appointments.push_back(appointment); }

    // Getter and setter for medical records
    const vector<MedicalRecord> &getMedicalRecords() const { return medicalRecords; }
    void addMedicalRecord(const MedicalRecord &medicalRecord) { medicalRecords.push_back(medicalRecord); }

    // Getter and setter for bills
    const vector<Billing> &getBills() const { return bills; }
    void addBill(const Billing &bill) { bills.push_back(bill); }

    // Getter and setter for lab
    Lab &getLab() { return lab; }

    // Getter and setter for inventory
    Inventory &getInventory() { return inventory; }

    string toString() const
    {
        string output = "Hospital Information:\n";
        // Add more details as needed
        return output;
    }

    ~Hospital()
    {
        de_alloc("Hospital");
    }
};

class TextFileReader
{
public:
    TextFileReader() 
    {
        new_alloc("TextFileReader");
    }
    
    vector<vector<string>> *readData(const string &filename)
    {

        vector<vector<string>> *data = new vector<vector<string>>;
        ifstream file(filename.c_str());

        if (!file.is_open())
        {
            throw runtime_error("Failed to open file: " + filename);
        }

        string line;
        while (getline(file, line))
        {
            if(line == "")
                continue;
            vector<string> row;
            stringstream ss(line);
            string item;

            while (getline(ss, item, ';'))
            {
                row.push_back(item);
            }
            data->push_back(row);
        }
        if(data->size() == 0)
            throw runtime_error("File is empty: " + filename);

        file.close();

        return data;
    }

    ~TextFileReader()
    {
        de_alloc("TextFileReader");
    }
};

class ObjectCache
{
    static ObjectCache *instance;
    std::map<std::string, Patient *> patients;
    std::map<std::string, Doctor *> doctors;
    std::map<std::string, Prescription *> prescriptions;
    
    ObjectCache() 
    {
        new_alloc("ObjectCache");
    }

public:
    static ObjectCache *getInstance()
    {
        if (instance == NULL)
            instance = new ObjectCache;
        return instance;
    }
    
    void addPatients(Patient *patient) { patients[patient->getPatientID()] = patient; }
    void addDoctors(Doctor *doctor) { doctors[doctor->getId()] = doctor; }
    void addPrescriptions(Prescription *prescription) { prescriptions[prescription->getPrescriptionID()] = prescription; }

    std::map<std::string, Patient *> *getPatients() { return &patients; }
    std::map<std::string, Doctor *> *getDoctors() { return &doctors; }
    std::map<std::string, Prescription *> *getPrescriptions() { return &prescriptions; }

    Patient *getPatient(const std::string &id) { return patients[id]; }
    Doctor *getDoctor(const std::string &id) { return doctors[id]; }
    Prescription *getPrescription(const std::string &id) { return prescriptions[id]; }

    std::vector<Patient *> *getPatientList()
    {
        std::vector<Patient *> *patientList = new std::vector<Patient *>(patients.size());
        for (auto it = patients.begin(); it != patients.end(); ++it)
        {
            patientList->push_back(it->second);
        }
        return patientList;
    }

    std::vector<Doctor *> *getDoctorList()
    {
        std::vector<Doctor *> *doctorList = new std::vector<Doctor *>(doctors.size());
        for (auto it = doctors.begin(); it != doctors.end(); ++it)
        {
            doctorList->push_back(it->second);
        }
        return doctorList;
    }

    std::vector<Prescription *> *getPrescriptionList()
    {
        std::vector<Prescription *> *prescriptionList = new std::vector<Prescription *>(prescriptions.size());
        for (auto it = prescriptions.begin(); it != prescriptions.end(); ++it)
        {
            prescriptionList->push_back(it->second);
        }
        return prescriptionList;
    }

    ~ObjectCache()
    {
        releasePatients();
        releaseDoctors();
        releasePrescriptions();

        de_alloc("ObjectCache");
    }

private:
    void releasePatients()
    {
        std::vector<Patient *> *dataObjects = this->getPatientList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = nullptr;
    }
    
    void releaseDoctors()
    {
        std::vector<Doctor *> *dataObjects = this->getDoctorList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = nullptr;
    }

    void releasePrescriptions()
    {
        std::vector<Prescription *> *dataObjects = this->getPrescriptionList();
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        delete dataObjects;
        dataObjects = nullptr;
    }
};

ObjectCache *ObjectCache::instance;

class Builder
{
protected:
    string filepath;
    virtual vector<DataObject *> *constructDataObject(vector<vector<string>> *data) = 0;

public:
    Builder()
    {
        new_alloc("Builder");
        filepath = "C:/Users/smhem/OneDrive/Desktop/NewHospital/";
    }

    vector<DataObject *> *makeDataObjectList()
{
    TextFileReader *textFileReader = new TextFileReader;
    try {
        vector<vector<string>> *data = textFileReader->readData(filepath);
        vector<DataObject *> *dataObjects = constructDataObject(data);

        return dataObjects;
    } catch(const runtime_error& e) {
        cout << "Caught a runtime error " << e.what() << endl;
        
      return nullptr; 
    }
}

    ~Builder()
    {
        de_alloc("Builder");
    }
};

class PatientBuilder : public Builder
{
public:
    PatientBuilder()
    {
        filepath += "patient.txt";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> >*data)
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());
        for (int i = 0; i < data->size(); ++i)
        {
            string &id = (*data)[i][0];

            string &name = (*data)[i][1];
            string &dateOfBirth = (*data)[i][2];
            string &gender = (*data)[i][3];
            string &mobile = (*data)[i][4];
            string &city = (*data)[i][5];

            Patient *patient = new Patient(id, name, dateOfBirth, gender, mobile, city);
            (*dataObjects)[i] = patient;

            ObjectCache::getInstance()->addPatients(patient);
            // }
        }
        return dataObjects;
    }
    ~PatientBuilder()
    {
        de_alloc("PatientBuilder");
    }
};

class DoctorBuilder : public Builder
{
public:
    DoctorBuilder()
    {
        new_alloc("DoctorBuilder");
        filepath += "Doctor.txt";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> > *data)
    {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());

        for (int i = 0; i < data->size(); ++i)
        {
            string &name = (*data)[i][1];
            string &dateOfBirth = (*data)[i][2];
            string &gender = (*data)[i][3];
            string &mobile = (*data)[i][4];
            string &city = (*data)[i][5];

            string &id = (*data)[i][0];
            string &specialization = (*data)[i][6];

            Doctor *doctor = new Doctor(name, dateOfBirth, gender, mobile, city, id, specialization);
            (*dataObjects)[i] = doctor;

            ObjectCache::getInstance()->addDoctors(doctor);
        }
        return dataObjects;
    }

    ~DoctorBuilder()
    {
        de_alloc("DoctorBuilder");
    }
};

class PrescriptionBuilder : public Builder {
public:
    PrescriptionBuilder() {
        new_alloc("PrescriptionBuilder");
        filepath += "prescription.txt";
    }

    vector<DataObject *> *constructDataObject(vector<vector<string> > *data) {
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());
        for (int i = 0; i < data->size(); ++i) {
            string& prescriptionID = (*data)[i][0];  
            string& patient = (*data)[i][1];        
            string& doctor = (*data)[i][2];            
            string& prescriptionDetails = (*data)[i][3];
            string& datePrescribed = (*data)[i][4];
            Prescription *prescription = new Prescription(prescriptionID, patient, doctor, prescriptionDetails, datePrescribed);
            (*dataObjects)[i] = prescription;

            ObjectCache::getInstance()->addPrescriptions(prescription); 
        }
        return dataObjects;
    }

    ~PrescriptionBuilder() {
        de_alloc("PrescriptionBuilder");
    }
};

class BuilderFactory
{
private:
    map<string, Builder *> builderMap;
    static BuilderFactory *instance;

    BuilderFactory()
    {
        builderMap["Patient"] = new PatientBuilder;
        builderMap["Doctor"] = new DoctorBuilder;
        builderMap["Prescription"] = new PrescriptionBuilder; 
    }

public:
    static BuilderFactory* getInstance()
    {
        if(instance == nullptr)
            instance = new BuilderFactory;
        return instance;
    }

    Builder *createBuilder(string className)
    {
        return builderMap[className];
    }

    ~BuilderFactory()
    {
        de_alloc("BuilderFactory");
    }
};

BuilderFactory *BuilderFactory::instance;

class Form
{
public:
    Form()
    {
        new_alloc("Form");
    }

    virtual void displayData(vector<DataObject *>* dataObjects) = 0;
    virtual void displayCacheData() = 0;

    ~Form()
    {
        de_alloc("Form");
    }
};

class PatientForm : public Form
{
    void displayPatient(Patient *patients)
    {
        cout << "Id: " << patients->getPatientID() << endl;
        cout << "Name: " << patients->getName() << endl;
        cout << "Date Of Birth: " << patients->getDateOfBirth() << endl;
        cout << "Gender: " << patients->getGender() << endl;
        cout << "Mobile: " << patients->getMobile() << endl;
        cout << "City: " << patients->getCity() << endl
        << endl;
    }

public:
    PatientForm()
    {
        new_alloc("PatientForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Patient's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dobj = (*dataObjects)[i];
            Patient *pt = dynamic_cast<Patient *>(dobj);
            displayPatient(pt);
        }
    }

    void displayCacheData()
    {
        map<string, Patient*>* patients = ObjectCache::getInstance()->getPatients();
        cout << "Patient's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (auto patient : (*patients))
        {
            cout << patient.second->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~PatientForm()
    {
        de_alloc("PatientForm");
    }
};

class DoctorForm : public Form
{
private:
    void displayDoctor(Doctor *doctor)
    {
        cout << "Name: " << doctor->getName() << endl;
        cout << "Date Of Birth: " << doctor->getDateOfBirth() << endl;
        cout << "Gender: " << doctor->getGender() << endl;
        cout << "Mobile: " << doctor->getMobile() << endl;
        cout << "City: " << doctor->getCity() << endl;
        cout << "Id: " << doctor->getId() << endl;
        cout << "Specialization: " << doctor->getSpecialization() << endl
        << endl;
    }

public:
    DoctorForm()
    {
        new_alloc("DoctorForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Doctor's Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dobj = (*dataObjects)[i];
            Doctor *dt = dynamic_cast<Doctor *>(dobj);
            displayDoctor(dt);
        }
    }

    void displayCacheData()
    {
        map<string, Doctor *>* Doctors = ObjectCache::getInstance()->getDoctors();
        cout << "Doctor's Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (auto doctor : (*Doctors))
        {
            cout << doctor.second->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~DoctorForm()
    {
        de_alloc("DoctorForm");
    }
};

class PrescriptionForm : public Form
{
private:
    void displayPrescription(Prescription *prescription)
    {
        cout << "Prescription ID: " << prescription->getPrescriptionID() << endl;
        cout << "Patient: " << prescription->getPatient() << endl;
        cout << "Doctor: " << prescription->getDoctor() << endl;
        cout << "Prescription Details: " << prescription->getPrescriptionDetails() << endl;
        cout << "Date Prescribed: " << prescription->getDatePrescribed() << endl << endl;
    }

public:
    PrescriptionForm()
    {
        new_alloc("PrescriptionForm");
    }

    void displayData(vector<DataObject *>* dataObjects)
    {
        cout << "Prescriptions Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            DataObject *dobj = (*dataObjects)[i];
            Prescription *prescription = dynamic_cast<Prescription *>(dobj);
            displayPrescription(prescription);
        }
    }

    void displayCacheData()
    {
        map<string, Prescription *> *prescriptions = ObjectCache::getInstance()->getPrescriptions();
        cout << "Prescriptions Cache Data: " << endl;
        cout << "-----------------------------------------------" << endl;
        for(auto prescription: (*prescriptions))
        {
            cout << prescription.second->toString() << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    ~PrescriptionForm()
    {
        de_alloc("PrescriptionForm");
    }
};


class FormFactory {
private:
    std::map<std::string, Form*> form;
    static FormFactory *instance;

    FormFactory() {
        // Private constructor to prevent external instantiation
        // Initialize the form map with form names and their corresponding instances
        form["Patient"] = new PatientForm;
        form["Doctor"] = new DoctorForm;
        form["Prescription"] = new PrescriptionForm;
    }

public:
    static FormFactory* getInstance() {
        if (instance == nullptr) {
            instance = new FormFactory;
        }
        return instance;
    }

    Form* createForm(std::string className) {
        // Check if the requested form exists in the map
        if (form.find(className) != form.end()) {
            return form[className];
        }
        return nullptr;  // Return nullptr if form is not found
    }

    ~FormFactory() {
        // Destructor to release resources (delete the created forms)
        for (auto it = form.begin(); it != form.end(); ++it) {
            delete it->second;  // Delete each form instance
        }
        form.clear();  // Clear the map
    }
};

FormFactory *FormFactory::instance;

void displayObjectCounter()
{
    map<string, int> counter = ObjectCounter::getInstance()->getCounter();

    cout << endl
         << "Classes Count: " << counter.size() << endl;
    cout << "------------------------------------" << endl;

    for (auto it = counter.begin(); it != counter.end(); ++it)
    {
        cout << it->first << "=" << it->second << endl;
    }
}

class PatientController
{
    static PatientController *instance;
    PatientController()
    {
        new_alloc("PatientController");
    }
public:
    static PatientController* getInstance()
    {
        if(instance == NULL)
            instance = new PatientController;
        return instance;
    }

    void execute()
    {
        Builder *builder1 = BuilderFactory::getInstance()->createBuilder("Patient");
        vector<DataObject *> *dataObjects1 = builder1->makeDataObjectList();

        // Patients data
        Form *form1 = FormFactory::getInstance()->createForm("Patient");
        form1->displayData(dataObjects1);

        cout << "-------------------------------------" << endl;
        form1->displayCacheData();
    }    

    ~PatientController()
    {
        de_alloc("PatientController");
    }
};

PatientController *PatientController::instance;

class DoctorController
{
    static DoctorController *instance;
    DoctorController()
    {
        new_alloc("DoctorController");
    }
public:
    static DoctorController* getInstance()
    {
        if(instance == NULL)
            instance = new DoctorController;
        return instance;
    }

    void execute()
    {
        // Doctors data
        Builder *builder2 = BuilderFactory::getInstance()->createBuilder("Doctor");
        vector<DataObject *> *dataObjects2 = builder2->makeDataObjectList();

        Form *form2 = FormFactory::getInstance()->createForm("Doctor");
        form2->displayData(dataObjects2);

        cout << "-------------------------------------" << endl;
        form2->displayCacheData();
    }

    ~DoctorController()
    {
        de_alloc("DoctorController");
    }
};

DoctorController *DoctorController::instance;

class PrescriptionController
{
    static PrescriptionController *instance;
    PrescriptionController()
    {
        new_alloc("PrescriptionController");
    }
public:
    static PrescriptionController* getInstance()
    {
        if(instance == NULL)
            instance = new PrescriptionController;
        return instance;
    }

    void execute()
    {
        // Prescription data
        Builder *builder = BuilderFactory::getInstance()->createBuilder("Prescription");
        vector<DataObject *> *dataObjects = builder->makeDataObjectList();

        Form *form = FormFactory::getInstance()->createForm("Prescription");
        form->displayData(dataObjects);

        cout << "-------------------------------------" << endl;
        form->displayCacheData();
    }

    ~PrescriptionController()
    {
        de_alloc("PrescriptionController");
    }
};

PrescriptionController *PrescriptionController::instance;


class HMSAppController
{
    static HMSAppController *instance;
    HMSAppController()
    {
        new_alloc("HMSAppController");
        #ifdef ENABLE_NEW_ALLOC
            displayObjectCounter();
        #endif
    }

public:
    static HMSAppController* getInstance()
    {
        if(instance == NULL)
            instance = new HMSAppController;
        return instance;
    }

void execute()
{
    PatientController::getInstance()->execute();
    DoctorController::getInstance()->execute();
    PrescriptionController::getInstance()->execute();  
}

~HMSAppController()
{
#ifdef ENABLE_NEW_ALLOC
    displayObjectCounter();
#endif

    delete ObjectCache::getInstance();
    delete BuilderFactory::getInstance();
    delete FormFactory::getInstance();

    delete PatientController::getInstance();
    delete DoctorController::getInstance();
    delete PrescriptionController::getInstance();  

    de_alloc("HMSAppController");
#ifdef ENABLE_NEW_ALLOC
    displayObjectCounter();
#endif
}
};
HMSAppController *HMSAppController::instance;
int main()
{
    HMSAppController::getInstance()->execute();
    delete ObjectCounter::getInstance();
    delete HMSAppController::getInstance();
    return 0;
}