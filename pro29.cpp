#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

using std::cout;
using std::cerr;
using std::endl;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;
using std::map;
using std::runtime_error;

#define ENABLE_NEW_ALLOC
#define ENABLE_DE_ALLOC

class ObjectCounter {
private:
    static ObjectCounter *instance;
    map<string, int> counter;

    ObjectCounter() {
        // Initialize counts for all classes
        counter["Builder"]=0;
        counter["BuilderFactory"]=0;
        counter["DataObject"] = 0;
        counter["Department"] = 0;
        counter["TenantBuilder"] = 0;
        counter["DepartmentController"] = 0;
        counter["DepartmentForm"] = 0;
        counter["Doctor"] = 0;
        counter["DoctorBuilder"] = 0;
        counter["DoctorController"] = 0;
        counter["DoctorForm"] = 0;
        counter["Form"] = 0;
        counter["FormFactory"] = 0;
        counter["Controller"] = 0;
        counter["ObjectCache"] = 0;
        counter["Patient"] = 0;
        counter["PatientForm"] = 0;  
        counter["Person"] = 0;
        counter["ShiftSchedule"] = 0;
        counter["Staff"] = 0;
        counter["TextFileReader"] = 0;
    }

public:
    static ObjectCounter *getInstance() {
        if (instance == nullptr) {
            instance = new ObjectCounter;
        }
        return instance;
    }

    void increment(const string& classname) {
        counter[classname]++;
    }

    void decrement(const string& classname) {
        counter[classname]--;
    }

    map<string, int> getCounter() const { return counter; }
};

ObjectCounter *ObjectCounter::instance = nullptr;
 
#ifdef ENABLE_NEW_ALLOC
void mem_new_alloc(string className)
{
    ObjectCounter::getInstance()->increment(className);
}
#endif
 
#ifdef ENABLE_DE_ALLOC
void mem_de_alloc(string className)
{
    ObjectCounter::getInstance()->decrement(className);
}
#endif
 
inline void new_alloc(string className)
{
#ifdef ENABLE_NEW_ALLOC
    mem_new_alloc(className);
#endif
}
 
inline void de_alloc(string className)
{
#ifdef ENABLE_DE_ALLOC
    mem_de_alloc(className);
#endif
}


class DataObject {
public:
    virtual string toString() const = 0;  
    virtual ~DataObject() {}              
};

class Person : public DataObject {
protected:
    string name;
    string dateOfBirth;
    string gender;
    string mobile;
    string city;

public:
    Person() {}

    Person(const string& name_, const string& dateOfBirth_, const string& gender_, const string& mobile_, const string& city_)
        : name(name_), dateOfBirth(dateOfBirth_), gender(gender_), mobile(mobile_), city(city_) {}

    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }   //when ever we call by value we end up calling copy constructor so we need to it by const reference

    string getDateOfBirth() const { return dateOfBirth; }
    void setDateOfBirth(const string& dateOfBirth) { this->dateOfBirth = dateOfBirth; }

    string getGender() const { return gender; }
    void setGender(const string& gender) { this->gender = gender; }

    string getMobile() const { return mobile; }
    void setMobile(const string& mobile) { this->mobile = mobile; }

    string getCity() const { return city; }
    void setCity(const string& city) { this->city = city; }

    virtual string toString() const  {
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

class Patient : public Person {
private:
    string patientID;
    vector<MedicalRecord> medicalRecords;

public:
    Patient() {
        new_alloc("Patient");
    }

    Patient(const string& patientID_, const string& name_, const string& dateOfBirth_, const string& gender_, const string& mobile_, const string& city_)
        : Person(name_, dateOfBirth_, gender_, mobile_, city_), patientID(patientID_) 
        {
          new_alloc("Patient");
       
    }

    ~Patient(){
        de_alloc("Patient");
    }

    string getPatientID() const { return patientID; }
    void setPatientID(const string& patientID) { this->patientID = patientID; }

    const vector<MedicalRecord>& getMedicalRecords() const { return medicalRecords; }
    void addMedicalRecord(const MedicalRecord& medicalRecord) { medicalRecords.push_back(medicalRecord); }

    virtual string toString() const ; 
};

class Staff : public Person {
protected:
    string id;

public:
    Staff() {}

    Staff(const string& name_, const string& dateOfBirth_, const string& gender_, const string& mobile_, const string& city_, const string& id_)
        : Person(name_, dateOfBirth_, gender_, mobile_, city_), id(id_) {}

    string getId() const { return id; }
    void setId(const string& id) { this->id = id; }

    virtual string toString() const  {
        stringstream ss;
        ss << "ID: " << id << "\n";
        ss << Person::toString();
        return ss.str();
    }
};

class AdministrativeStaff : public Staff {
private:
    string designation;

public:
    AdministrativeStaff() {}

    AdministrativeStaff(const string& name_, const string& dateOfBirth_, const string& gender_, const string& mobile_, const string& city_, const string& id_, const string& designation_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), designation(designation_) {}

    string getDesignation() const { return designation; }
    void setDesignation(const string& designation) { this->designation = designation; }

    virtual string toString() const  {
        stringstream ss;
        ss << "Designation: " << designation << "\n";
        ss << Staff::toString();
        return ss.str();
    }
};

class MedicalRecord : public DataObject {
private:
    string recordID;
    string patientID;
    string doctorID;
    string diagnosis;
    string treatment;
    string medications;
    string labResults;

public:
    MedicalRecord() {
        new_alloc("MedicalRecord");
    }

    MedicalRecord(const string& recordID_, const string& patientID_, const string& doctorID_, const string& diagnosis_, const string& treatment_, const string& medications_, const string& labResults_)
        : recordID(recordID_), patientID(patientID_), doctorID(doctorID_), diagnosis(diagnosis_), treatment(treatment_), medications(medications_), labResults(labResults_) {
            new_alloc("MedicalRecord");
    }

    ~MedicalRecord(){
        de_alloc("MedicalRecord");
    }

    string getRecordID() const { return recordID; }
    void setRecordID(const string& recordID) { this->recordID = recordID; }

    string getPatientID() const { return patientID; }
    void setPatientID(const string& patientID) { this->patientID = patientID; }

    string getDoctorID() const { return doctorID; }
    void setDoctorID(const string& doctorID) { this->doctorID = doctorID; }

    string getDiagnosis() const { return diagnosis; }
    void setDiagnosis(const string& diagnosis) { this->diagnosis = diagnosis; }

    string getTreatment() const { return treatment; }
    void setTreatment(const string& treatment) { this->treatment = treatment; }

    string getMedications() const { return medications; }
    void setMedications(const string& medications) { this->medications = medications; }

    string getLabResults() const { return labResults; }
    void setLabResults(const string& labResults) { this->labResults = labResults; }

    string toString() const  {
        return "Medical Record ID: " + recordID + ", Diagnosis: " + diagnosis + ", Treatment: " + treatment;
    }
};

string Patient::toString() const {
    stringstream ss;
    ss << "Patient ID: " << patientID << "\n";
    ss << Person::toString();
    for (const auto& record : medicalRecords) {
        ss << "Medical Record: " << record.toString() << "\n";
    }
    return ss.str();
}
    
class ShiftDetails : public DataObject {
private:
    string shiftType;
    string timeSlotRange;

public:
    ShiftDetails() {}

    ShiftDetails(const string& shiftType_, const string& timeSlotRange_)
        : shiftType(shiftType_), timeSlotRange(timeSlotRange_) {}

    // Getters and setters
    string getShiftType() const { return shiftType; }
    void setShiftType(const string& shiftType) { this->shiftType = shiftType; }

    string getTimeSlotRange() const { return timeSlotRange; }
    void setTimeSlotRange(const string& timeSlotRange) { this->timeSlotRange = timeSlotRange; }

    virtual string toString() const  {
        stringstream ss;
        ss << "Shift Type: " << shiftType << "\n";
        ss << "Time Slot Range: " << timeSlotRange << "\n";
        return ss.str();
    }
};

class ShiftSchedule : public DataObject {
private:
    map<string, ShiftDetails> shifts; // Maps month to shift details

public:
    ShiftSchedule() {}

    void addShift(const string& month, const ShiftDetails& shiftDetails) {
        shifts[month] = shiftDetails;
    }

    ShiftDetails getShift(const string& month) const {
        auto it = shifts.find(month);
        if (it != shifts.end()) {
            return it->second;
        }
        return ShiftDetails("", "");
    }

    virtual string toString() const  {
        stringstream ss;
        for (const auto& entry : shifts) {
            ss << "Month: " << entry.first << "\n";
            ss << entry.second.toString() << "\n"; // Use the toString() method of ShiftDetails
        }
        return ss.str();
    }
};

class Doctor : public Staff {
private:
    string specialization;
    ShiftSchedule shiftSchedule;

public:
    Doctor() :  shiftSchedule() {
        new_alloc("Doctor");
    }

    Doctor(const string& name_, const string& dateOfBirth_, const string& gender_, const string& mobile_, const string& city_, const string& id_, const string& specialization_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), specialization(specialization_), shiftSchedule() {
            new_alloc("Doctor");
        }

    ~Doctor(){
        de_alloc("Doctor");
    }

    string getSpecialization() const { return specialization; }
    void setSpecialization(const string& specialization) { this->specialization = specialization; }

    const ShiftSchedule& getShiftSchedule() const { return shiftSchedule; }
    void setShiftSchedule(const ShiftSchedule& schedule) { shiftSchedule = schedule; }

    virtual string toString() const  {
        stringstream ss;
        ss << "Specialization: " << specialization << "\n";
        ss << shiftSchedule.toString(); // Use the toString() method of ShiftSchedule
        ss << Staff::toString();
        return ss.str();
    }
};

class Nurse : public Staff {
private:
    string specialization;
    ShiftSchedule shiftSchedule;

public:
    Nurse() : shiftSchedule() {}

    Nurse(const string& name_, const string& dateOfBirth_, const string& gender_, const string& mobile_, const string& city_, const string& id_, const string& specialization_)
        : Staff(name_, dateOfBirth_, gender_, mobile_, city_, id_), specialization(specialization_), shiftSchedule() {}

    string getSpecialization() const { return specialization; }
    void setSpecialization(const string& specialization) { this->specialization = specialization; }

    const ShiftSchedule& getShiftSchedule() const { return shiftSchedule; }
    void setShiftSchedule(const ShiftSchedule& schedule) { shiftSchedule = schedule; }

    virtual string toString() const  {
        stringstream ss;
        ss << "Specialization: " << specialization << "\n";
        ss << shiftSchedule.toString();
        ss << Staff::toString();
        return ss.str();
    }
};

class Department : public DataObject {
private:
    string departmentID;
    string name;
    string description;
    Doctor* headOfDepartment;

public:
    Department() :  headOfDepartment(nullptr) {}

    Department(const string& departmentID_, const string& name_, const string& description_, Doctor* headOfDepartment_)
        : departmentID(departmentID_), name(name_), description(description_), headOfDepartment(headOfDepartment_) {}

    // Setter and Getter pairs
    void setDepartmentID(const string& departmentID) { this->departmentID = departmentID; }
    string getDepartmentID() const { return departmentID; }

    void setName(const string& name) { this->name = name; }
    string getName() const { return name; }

    void setDescription(const string& description) { this->description = description; }
    string getDescription() const { return description; }

    void setHeadOfDepartment(Doctor* headOfDepartment) { this->headOfDepartment = headOfDepartment; }
    Doctor* getHeadOfDepartment() const { return headOfDepartment; }

    virtual string toString() const  {
        stringstream ss;
        ss << "Department ID: " << departmentID << "\n";
        ss << "Name: " << name << "\n";
        ss << "Description: " << description << "\n";
        if (headOfDepartment) {
            ss << "Head of Department: " << headOfDepartment->getName() << "\n";
        }
        return ss.str();
    }
};

class Appointment : public DataObject {
private:
    string appointmentID;
    Person* patient;
    Person* doctor;
    string dateTime;
    string reason;

public:
    Appointment() : patient(nullptr), doctor(nullptr) {}

    Appointment(const string& appointmentID_, Person* patient_, Person* doctor_, const string& dateTime_, const string& reason_)
        : appointmentID(appointmentID_), patient(patient_), doctor(doctor_), dateTime(dateTime_), reason(reason_) {}

    // Setter and Getter pairs
    void setAppointmentID(const string& appointmentID) { this->appointmentID = appointmentID; }
    string getAppointmentID() const { return appointmentID; }

    void setPatient(Person* patient) { this->patient = patient; }
    Person* getPatient() const { return patient; }

    void setDoctor(Person* doctor) { this->doctor = doctor; }
    Person* getDoctor() const { return doctor; }

    void setDateTime(const string& dateTime) { this->dateTime = dateTime; }
    string getDateTime() const { return dateTime; }

    void setReason(const string& reason) { this->reason = reason; }
    string getReason() const { return reason; }

    virtual string toString() const  {  
        stringstream ss;
        ss << "Appointment ID: " << appointmentID << "\n";
        ss << "Patient: " << patient->getName() << "\n";
        ss << "Doctor: " << doctor->getName() << "\n";
        ss << "Date & Time: " << dateTime << "\n";
        ss << "Reason: " << reason << "\n";
        return ss.str();
    }
};

class Billing : public DataObject {
private:
    string billID;
    Person* patient;
    vector<string> servicesProvided;
    double cost;
    bool paymentStatus;

public:
    Billing() : patient(nullptr), cost(0), paymentStatus(false) {}

    Billing(const string& billID_, Person* patient_, const vector<string>& servicesProvided_, double cost_)
        : billID(billID_), patient(patient_), servicesProvided(servicesProvided_), cost(cost_), paymentStatus(false) {}

    // Setter and Getter pairs
    void setBillID(const string& billID) { this->billID = billID; }
    string getBillID() const { return billID; }

    void setPatient(Person* patient) { this->patient = patient; }
    Person* getPatient() const { return patient; }

    void setServicesProvided(const vector<string>& servicesProvided) { this->servicesProvided = servicesProvided; }
    const vector<string>& getServicesProvided() const { return servicesProvided; }

    void setCost(double cost) { this->cost = cost; }
    double getCost() const { return cost; }

    void setPaymentStatus(bool status) { this->paymentStatus = status; }
    bool getPaymentStatus() const { return paymentStatus; }

    virtual string toString() const  {
        stringstream ss;
        ss << "Bill ID: " << billID << "\n";
        ss << "Patient: " << patient->getName() << "\n";
        ss << "Services Provided:\n";
        for (const auto& service : servicesProvided) {
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
    Prescription() 
    {    new_alloc("Prescription");//function calls 
     }

    Prescription(const string& prescriptionID_, const string& patient_, const string& doctor_, const string& prescriptionDetails_, const string& datePrescribed_)
        : prescriptionID(prescriptionID_), patient(patient_), doctor(doctor_), prescriptionDetails(prescriptionDetails_), datePrescribed(datePrescribed_) 
        {
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

    string toString() const  
    {
        stringstream ss;
        ss << "Prescription ID: " << prescriptionID<<"\n";
        ss<< ", Patient: " << patient <<"\n";
        ss<< ", Doctor: " << doctor<<"\n";
        ss<< ", Prescription Details: " << prescriptionDetails<<"\n";    
        ss<< ", Date Prescribed: " << datePrescribed<<"\n";
        return ss.str();
    }
};

class Item : public DataObject {
private:
    string itemID;
    string itemName;
    string expiryDate;

public:
    Item(const string& itemID_, const string& itemName_, const string& expiryDate_)
        : itemID(itemID_), itemName(itemName_), expiryDate(expiryDate_) {}

    string getItemID() const { return itemID; }
    void setItemID(const string& itemID) { this->itemID = itemID; }

    string getItemName() const { return itemName; }
    void setItemName(const string& itemName) { this->itemName = itemName; }

    string getExpiryDate() const { return expiryDate; }
    void setExpiryDate(const string& expiryDate) { this->expiryDate = expiryDate; }

    string toString() const  {
        return "Item ID: " + itemID + ", Item Name: " + itemName + ", Expiry Date: " + expiryDate;
    }
};

class Supplier : public DataObject {
private:
    string supplierID;
    string name;
    string mobile;

public:
    Supplier(const string& supplierID_, const string& name_, const string& mobile_)
        : supplierID(supplierID_), name(name_), mobile(mobile_) {}

    string getSupplierID() const { return supplierID; }
    void setSupplierID(const string& supplierID) { this->supplierID = supplierID; }

    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }

    string getMobile() const { return mobile; }
    void setMobile(const string& mobile) { this->mobile = mobile; }

    string toString() const  {
        return "Supplier ID: " + supplierID + ", Name: " + name + ", Mobile: " + mobile;
    }
};

class StockedItem : public DataObject {
private:
    string dtPurchase;
    int quantity;
    Item* item;
    Supplier* supplier;

public:
    StockedItem(const string& dtPurchase_, int quantity_, Item* item_, Supplier* supplier_)
        : dtPurchase(dtPurchase_), quantity(quantity_), item(item_), supplier(supplier_) {}

    string getDtPurchase() const { return dtPurchase; }
    void setDtPurchase(const string& dtPurchase) { this->dtPurchase = dtPurchase; }

    int getQuantity() const { return quantity; }
    void setQuantity(int quantity) { this->quantity = quantity; }

    Item* getItem() const { return item; }
    void setItem(Item* item) { this->item = item; }

    Supplier* getSupplier() const { return supplier; }
    void setSupplier(Supplier* supplier) { this->supplier = supplier; }

    string toString() const  {
        return "Date of Purchase: " + dtPurchase + ", Quantity: " + std::to_string(quantity);
    }
};

class Inventory {
private:
    vector<StockedItem> stockedItems;

public:
    void addStockedItem(const StockedItem& stockedItem) {
        stockedItems.push_back(stockedItem);
    }

    const vector<StockedItem>& getStockedItems() const {
        return stockedItems;
    }
};

class Test : public DataObject {
private:
    string testID;
    Patient* patient;
    AdministrativeStaff* technician;
    string testType;
    string result;

public:
    Test() : patient(nullptr), technician(nullptr) {}

    Test(const string& testID_, Patient* patient_, AdministrativeStaff* technician_, const string& testType_, const string& result_)
        : testID(testID_), patient(patient_), technician(technician_), testType(testType_), result(result_) {}

    string getTestID() const { return testID; }
    void setTestID(const string& testID) { this->testID = testID; }

    Patient* getPatient() const { return patient; }
    void setPatient(Patient* patient) { this->patient = patient; }

    AdministrativeStaff* getTechnician() const { return technician; }
    void setTechnician(AdministrativeStaff* technician) { this->technician = technician; }

    string getTestType() const { return testType; }
    void setTestType(const string& testType) { this->testType = testType; }

    string getResult() const { return result; }
    void setResult(const string& result) { this->result = result; }

    string toString() const  {
        return "Test ID: " + testID + ", Test Type: " + testType + ", Result: " + result;
    }
};

class Lab : public DataObject {
private:
    vector<Test> tests;

public:
    const vector<Test>& getTests() const { return tests; }

    void addTest(const Test& test) {
        tests.push_back(test);
    }

    string toString() const  {
        string output = "Lab Tests:\n";
        for (const auto& test : tests) {
            output += test.toString() + "\n";
        }
        return output;
    }
};

class Hospital : public DataObject {
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
    Hospital() {} // Default constructor

    // Getter and setter for departments
    const vector<Department>& getDepartments() const { return departments; }
    void addDepartment(const Department& department) { departments.push_back(department); }

    // Getter and setter for doctors
    const vector<Doctor>& getDoctors() const { return doctors; }
    void addDoctor(const Doctor& doctor) { doctors.push_back(doctor); }

    // Getter and setter for nurses
    const vector<Nurse>& getNurses() const { return nurses; }
    void addNurse(const Nurse& nurse) { nurses.push_back(nurse); }

    // Getter and setter for administrative staff
    const vector<AdministrativeStaff>& getAdministrativeStaff() const { return administrativeStaff; }
    void addAdministrativeStaff(const AdministrativeStaff& staff) { administrativeStaff.push_back(staff); }

    // Getter and setter for patients
    const vector<Patient>& getPatients() const { return patients; }
    void addPatient(const Patient& patient) { patients.push_back(patient); }

    // Getter and setter for appointments
    const vector<Appointment>& getAppointments() const { return appointments; }
    void addAppointment(const Appointment& appointment) { appointments.push_back(appointment); }

    // Getter and setter for medical records
    const vector<MedicalRecord>& getMedicalRecords() const { return medicalRecords; }
    void addMedicalRecord(const MedicalRecord& medicalRecord) { medicalRecords.push_back(medicalRecord); }

    // Getter and setter for bills
    const vector<Billing>& getBills() const { return bills; }
    void addBill(const Billing& bill) { bills.push_back(bill); }

    // Getter and setter for lab
    Lab& getLab() { return lab; }

    // Getter and setter for inventory
    Inventory& getInventory() { return inventory; }

    string toString() const  {
        string output = "Hospital Information:\n";
        // Add more details as needed
        return output;
    }
};

class TextFileReader {
public:
    TextFileReader() {}
    vector<vector<string> >* readData(const string& filename) {
        vector<vector<string> >* data = new vector<vector<string>>;
        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + filename);
        }
        string line;
        while (getline(file, line)) {

            vector<string> row;
            stringstream ss(line);
            string item;

            while (getline(ss, item, ';')) {
                row.push_back(item);
                
            }
            data->push_back(row);
        }
        file.close();
        return data;
    }
};

class PrescriptionBuilder {
public:
    PrescriptionBuilder() {}

    vector<Prescription*>* makePrescriptionData() {
        TextFileReader* tfr = new TextFileReader;
        vector<vector<string>>* data = tfr->readData("Prescription.txt");
        vector<Prescription*>* prescriptions = new vector<Prescription*>;
        for (int i = 0; i < data->size(); ++i) {
            vector<string> row = (*data)[i];
            Prescription* pr = new Prescription(row[0], row[1], row[2], row[3], row[4]);
            prescriptions->push_back(pr);
        }
        delete data; 
        return prescriptions;
    }
};

class PrescriptionForm {
public:
    void display(vector<Prescription*>* prescriptionData) 
    {
        for (int i = 0; i < prescriptionData->size(); i++) 
        {
            cout << "prescriptionID: " << (*prescriptionData)[i]->getPrescriptionID() << endl;
            cout << "Patient " << (*prescriptionData)[i]->getPatient() << endl;
            cout << "Doctor " << (*prescriptionData)[i]->getDoctor() << endl;
            cout << "prescriptionDetails " << (*prescriptionData)[i]->getPrescriptionDetails() << endl;
            cout << "datePrescribed " << (*prescriptionData)[i]->getDatePrescribed() << endl;
            cout << endl;
        }
    }
};
class DoctorBuilder{
public:
    DoctorBuilder(){}
    vector<Doctor*>* makeDoctorData()
    {
        TextFileReader *tfr = new TextFileReader;
        vector<vector<string>>* data = tfr->readData("Doctor.txt");
        vector<Doctor*>* doctors = new vector<Doctor*>(data->size());
        for(int i=0;i<data->size();++i)
        {
            vector<string> row = (*data)[i];
            Doctor* d = new Doctor(row[1],row[2],row[3],row[4],row[5],row[0],row[6]);
            (*doctors)[i]=d;
        }
        return doctors;
    }
};

class DoctorForm{
public:
    void display(vector<Doctor*>* doctorData)
    {
        for(int i=0;i<doctorData->size();i++)
        {
            cout<<"Doctor Id : "<< (*doctorData)[i]->getId()<<endl;
            cout<<"Name : "<< (*doctorData)[i]->getName()<<endl;
            cout<<"DOB : "<< (*doctorData)[i]->getDateOfBirth() <<endl;
            cout<<"Gender : "<< (*doctorData)[i]->getGender() <<endl;
            cout<<"Mobile Number : "<< (*doctorData)[i]->getMobile() <<endl;
            cout<<"City : "<< (*doctorData)[i]->getCity() <<endl;
            cout<<"Specialization: "<< (*doctorData)[i]->getSpecialization() <<endl;
            cout<<endl;
        }
        
       
    }
};

class PatientBuilder {
public:
    PatientBuilder(){}
    vector<Patient*>* makePatientData() 
    {
        TextFileReader *tfr = new TextFileReader;
        vector<vector<string>>* patientData = tfr->readData("patient.txt");
        vector<Patient*>* patients = new vector<Patient*>(patientData->size());
        for (int i = 0; i < patientData->size(); ++i) {
            vector<string> row = (*patientData)[i];
            Patient* p = new Patient(row[0], row[1], row[2], row[3], row[4], row[5]);
            (*patients)[i] = p;
        }
        return patients;
    }
};

class PatientForm {
public:
    
    void display(vector<Patient*>* patientData) 
    {
        for (int i = 0; i < patientData->size(); i++) {
            cout << "Patient ID: " << (*patientData)[i]->getPatientID() << endl;
            cout << "Name: " << (*patientData)[i]->getName() << endl;
            cout << "Date of Birth: " << (*patientData)[i]->getDateOfBirth() << endl;
            cout << "Gender: " << (*patientData)[i]->getGender() << endl;
            cout << "Mobile: " << (*patientData)[i]->getMobile() << endl;
            cout << "City: " << (*patientData)[i]->getCity() << endl;
            cout << endl;
        }
    }
};
///////////////////
void displayObjCounter()
{
    map<string, int> counter = ObjectCounter::getInstance()->getCounter();

    cout << endl;
    cout << "Classes count: " << counter.size() << endl;
    cout << "------------" << endl;

    for (const auto& entry : counter) {
        cout << entry.first << " = " << entry.second << endl;
    }
}


void appController() {
    DoctorBuilder* doctorBuilder = new DoctorBuilder;
    vector<Doctor*>* doctorData = doctorBuilder->makeDoctorData();

    DoctorForm* doctorForm = new DoctorForm;
    doctorForm->display(doctorData);

    PatientBuilder* patientBuilder = new PatientBuilder;
    vector<Patient*>* patients = patientBuilder->makePatientData();

    PatientForm* patientForm = new PatientForm;
    patientForm->display(patients);

    PrescriptionBuilder* prescriptionBuilder = new PrescriptionBuilder;
    vector<Prescription*>* prescriptionData = prescriptionBuilder->makePrescriptionData();

    PrescriptionForm* prescriptionForm = new PrescriptionForm;
    prescriptionForm->display(prescriptionData);

    // Deallocate memory
    delete doctorBuilder;
    for (int i = 0; i < doctorData->size(); ++i) {
        delete (*doctorData)[i];
    }
    delete doctorData;

    delete patientBuilder;
    for (int i = 0; i < patients->size(); ++i) {
        delete (*patients)[i];
    }
    delete patients;

    delete prescriptionBuilder;
    for (int i = 0; i < prescriptionData->size(); ++i) {
        delete (*prescriptionData)[i];
    }
    delete prescriptionData;

    #ifdef ENABLE_NEW_ALLOC
        displayObjCounter();
    #endif
}


int main()
{
    appController();
    return 0;
}
