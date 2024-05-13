


class ObjectCounter {
private:
    static ObjectCounter *instance; // Static variable to hold the single instance of ObjectCounter
    map<string, int> counter; // Map to store counts for different class names

    ObjectCounter() {
        // Constructor to initialize counts for all classes to zero
        counter["Builder"] = 0;
        counter["BuilderFactory"] = 0;
        // Other class names and their initial counts are set here
    }

public:
    // Static method to get the single instance of ObjectCounter
    static ObjectCounter *getInstance() {
        // If the instance is null (first time), create a new instance
        if (instance == nullptr) {
            instance = new ObjectCounter;
        }
        // Return the single instance
        return instance;
    }

    // Method to increment the count for a specific class name
    void increment(const string& classname) {
        counter[classname]++;
    }

    // Method to decrement the count for a specific class name
    void decrement(const string& classname) {
        counter[classname]--;
    }

    // Method to get the entire counter map
    map<string, int> getCounter() const { return counter; }
};

// Initialize the static instance variable of ObjectCounter
ObjectCounter *ObjectCounter::instance = nullptr;

#ifdef ENABLE_NEW_ALLOC
// Function to increment the count for a given class name when new allocation happens
void mem_new_alloc(string className) {
    ObjectCounter::getInstance()->increment(className);
}
#endif

#ifdef ENABLE_DE_ALLOC
// Function to decrement the count for a given class name when deallocation happens
void mem_de_alloc(string className) {
    ObjectCounter::getInstance()->decrement(className);
}
#endif

// Inline function to call the memory allocation count increment function
inline void new_alloc(string className) {
#ifdef ENABLE_NEW_ALLOC
    mem_new_alloc(className);
#endif
}

// Inline function to call the memory deallocation count decrement function
inline void de_alloc(string className) {
#ifdef ENABLE_DE_ALLOC
    mem_de_alloc(className);
#endif
}


class Prescription : public DataObject {
private:
    // Private member variables to hold prescription data
    string prescriptionID;
    string patient;
    string doctor;
    string prescriptionDetails;
    string datePrescribed;

public:
    // Default constructor for Prescription class
    Prescription() {
        new_alloc("Prescription"); // Increment the allocation count for Prescription objects
    }

    // Parameterized constructor for Prescription class
    Prescription(const string& prescriptionID_, const string& patient_, const string& doctor_,
                 const string& prescriptionDetails_, const string& datePrescribed_)
        : prescriptionID(prescriptionID_), patient(patient_), doctor(doctor_),
          prescriptionDetails(prescriptionDetails_), datePrescribed(datePrescribed_) {
        new_alloc("Prescription"); // Increment the allocation count for Prescription objects
    }

    // Destructor for Prescription class
    ~Prescription() {
        de_alloc("Prescription"); // Decrement the allocation count for Prescription objects
    }

    // Getter and setter methods for prescriptionID
    string getPrescriptionID() const { return prescriptionID; }
    void setPrescriptionID(const string& prescriptionID) { this->prescriptionID = prescriptionID; }

    // Getter and setter methods for patient
    string getPatient() const { return patient; }
    void setPatient(const string& patient) { this->patient = patient; }

    // Getter and setter methods for doctor
    string getDoctor() const { return doctor; }
    void setDoctor(const string& doctor) { this->doctor = doctor; }

    // Getter and setter methods for prescriptionDetails
    string getPrescriptionDetails() const { return prescriptionDetails; }
    void setPrescriptionDetails(const string& prescriptionDetails) { this->prescriptionDetails = prescriptionDetails; }

    // Getter and setter methods for datePrescribed
    string getDatePrescribed() const { return datePrescribed; }
    void setDatePrescribed(const string& datePrescribed) { this->datePrescribed = datePrescribed; }

    // Method to convert Prescription object to string format
    string toString() const {
        stringstream ss;//used to read and sright string
        ss << "Prescription ID: " << prescriptionID << "\n"; // Append prescription ID to stringstream
        ss << ", Patient: " << patient << "\n"; // Append patient info to stringstream
        ss << ", Doctor: " << doctor << "\n"; // Append doctor info to stringstream
        ss << ", Prescription Details: " << prescriptionDetails << "\n"; // Append prescription details to stringstream
        ss << ", Date Prescribed: " << datePrescribed << "\n"; // Append date prescribed to stringstream
        return ss.str(); // Convert stringstream to string and return
    }
};
class Prescription : public DataObject {: Declares a class named Prescription that inherits from DataObject.
private:: Marks the following members as private, accessible only within the class.
string prescriptionID;: Declares a private member variable to store the prescription ID.
Similar declarations for patient, doctor, prescriptionDetails, and datePrescribed.
public:: Marks the following members as public, accessible from outside the class.
Prescription() { ... }: Default constructor that increments the allocation count for Prescription objects.
Prescription(...){ ... }: Parameterized constructor that initializes member variables and increments allocation count.
~Prescription() { ... }: Destructor that decrements the allocation count for Prescription objects.
Getter and setter methods (get and set) for each member variable.
string toString() const { ... }: Method to convert a Prescription object to a string format containing all prescription details.




class TextFileReader {
public:
    TextFileReader() {}

    // Read data from a text file and return a vector of vector of strings
    // Each inner vector represents a row in the text file
    vector<vector<string>> readData(const string& filename) {
        vector<vector<string>> data; // Vector to store the read data
        ifstream file(filename); // Open the file

        if (!file.is_open()) { // Check if file opening failed
            cerr << "Failed to open file: " << filename << endl;//cerr used write a error message
            // You can handle this error by throwing an exception if needed
            // throw runtime_error("Failed to open file: " + filename);
            return data; // Return empty data if file opening failed
        }

        string line; // String to store each line read from the file
        while (getline(file, line)) { // Read each line from the file
            if (line.empty()) // Skip empty lines
                continue;

            vector<string> row; // Vector to store data of each row
            stringstream ss(line); // Create a stringstream from the line
            string item; // String to store each item separated by a delimiter

            while (getline(ss, item, ';')) { // Read items separated by ';'
                row.push_back(item); // Add item to the row vector
            }

            data.push_back(row); // Add the row to the data vector
        }

        file.close(); // Close the file after reading
        return data; // Return the read data
    }
};


class PrescriptionBuilder {
public:
    PrescriptionBuilder() {} // Default constructor

    vector<Prescription*>* makePrescriptionData() { // Function to create Prescription data from a text file
        TextFileReader* tfr = new TextFileReader; // Create an instance of TextFileReader
        vector<vector<string>>* data = tfr->readData("Prescription.txt"); // Read data from "Prescription.txt"
        vector<Prescription*>* prescriptions = new vector<Prescription*>; // Create a vector to hold Prescription objects
        for (int i = 0; i < data->size(); ++i) { // Iterate through the data rows
            vector<string> row = (*data)[i]; // Get a row from the data
            Prescription* pr = new Prescription(row[0], row[1], row[2], row[3], row[4]); // Create a Prescription object from row data
            prescriptions->push_back(pr); // Add the Prescription object to the vector
        }
        delete data; // Free the memory allocated for data vector
        return prescriptions; // Return the vector of Prescription objects
    }
};

class PrescriptionForm {
public:
    void display(vector<Prescription*>* prescriptionData) { // Function to display Prescription data
        for (int i = 0; i < prescriptionData->size(); i++) { // Iterate through the vector of Prescription objects
            cout << "prescriptionID: " << (*prescriptionData)[i]->getPrescriptionID() << endl; // Display prescription ID
            cout << "Patient " << (*prescriptionData)[i]->getPatient() << endl; // Display patient info
            cout << "Doctor " << (*prescriptionData)[i]->getDoctor() << endl; // Display doctor info
            cout << "prescriptionDetails " << (*prescriptionData)[i]->getPrescriptionDetails() << endl; // Display prescription details
            cout << "datePrescribed " << (*prescriptionData)[i]->getDatePrescribed() << endl; // Display date prescribed
            cout << endl; // Add an empty line for formatting
        }
    }
};


void displayObjCounter()
{
    // Get the counter map from ObjectCounter instance
    map<string, int> counter = ObjectCounter::getInstance()->getCounter();

    cout << endl;
    cout << "Classes count: " << counter.size() << endl; // Print the number of classes counted
    cout << "------------" << endl;

    // Iterate through the counter map and print class counts
    for (const auto& entry : counter) {
        cout << entry.first << " = " << entry.second << endl; // Print class name and its count
    }
}

void appController() {
    // Create an instance of PrescriptionBuilder to build Prescription data
    PrescriptionBuilder* ppprescriptionBuilder = new qqPrescriptionBuilder;
    //This line creates a pointer named ppprescriptionBuilder that points to a new instance of the qqPrescriptionBuilder class allocated on the heap.
    // Get the Prescription data from the builder
    vector<Prescription*>* prescriptionData = prescriptionBuilder->makePrescriptionData();
//This line assigns the result of the makePrescriptionData method of prescriptionBuilder to a pointer to a vector of pointers to Prescription objects named prescriptionData.

    // Create an instance of PrescriptionForm to display Prescription data
    PrescriptionForm* prescriptionForm = new PrescriptionForm;
    // Display the Prescription data using the form
    prescriptionForm->display(prescriptionData);

    // Delete the PrescriptionBuilder instance
    delete prescriptionBuilder;
    
    // Delete each Prescription object in the vector
    for (int i = 0; i < prescriptionData->size(); ++i) {
        delete (*prescriptionData)[i];
    }
    // Delete the Prescription data vector itself
    delete prescriptionData;

    // Display object counter if ENABLE_NEW_ALLOC is defined
    #ifdef ENABLE_NEW_ALLOC
        displayObjCounter();
    #endif
}


----------------------------------------------------------------------------------------------------------------------------
// Define a class named ObjectCache
// Define a class named ObjectCache
class ObjectCache
{
    // Declare a static pointer to an ObjectCache instance
    static ObjectCache *instance;

    // Declare maps to store pointers to Patient, Doctor, and Prescription objects
    std::map<std::string, Patient *> patients;
    std::map<std::string, Doctor *> doctors;
    std::map<std::string, Prescription *> prescriptions;

    // Constructor for ObjectCache
    ObjectCache() 
    {
        // Call new_alloc() function with "ObjectCache" as an argument
        new_alloc("ObjectCache");
    }

public:
    // Static method to get the ObjectCache instance
    static ObjectCache *getInstance()
    {
        // Check if instance is NULL
        if (instance == NULL)
            // Create a new ObjectCache instance using new keyword
            instance = new ObjectCache;
        // Return the ObjectCache instance
        return instance;
    }

    // Methods to add objects to respective maps
    void addPatients(Patient *patient) { patients[patient->getPatientID()] = patient; }
    void addDoctors(Doctor *doctor) { doctors[doctor->getId()] = doctor; }
    void addPrescriptions(Prescription *prescription) { prescriptions[prescription->getPrescriptionID()] = prescription; }

    // Methods to get pointers to the maps
    std::map<std::string, Patient *> *getPatients() { return &patients; }
    std::map<std::string, Doctor *> *getDoctors() { return &doctors; }
    std::map<std::string, Prescription *> *getPrescriptions() { return &prescriptions; }

    // Methods to get objects from the maps using IDs
    Patient *getPatient(const std::string &id) { return patients[id]; }
    Doctor *getDoctor(const std::string &id) { return doctors[id]; }
    Prescription *getPrescription(const std::string &id) { return prescriptions[id]; }

    // Methods to get lists of objects from the maps
    std::vector<Patient *> *getPatientList()
    {
        // Create a new vector to store patients with the size equal to the number of patients
        std::vector<Patient *> *patientList = new std::vector<Patient *>(patients.size());
        // Loop through patients map and add pointers to the vector
        for (auto it = patients.begin(); it != patients.end(); ++it)
        {
            patientList->push_back(it->second);
        }
        // Return the vector
        return patientList;
    }

    // Similar methods for doctors and prescriptions
    std::vector<Doctor *> *getDoctorList()
    {
        // Create a new vector to store doctors with the size equal to the number of doctors
        std::vector<Doctor *> *doctorList = new std::vector<Doctor *>(doctors.size());
        // Loop through doctors map and add pointers to the vector
        for (auto it = doctors.begin(); it != doctors.end(); ++it)
        {
            doctorList->push_back(it->second);
        }
        // Return the vector
        return doctorList;
    }

    std::vector<Prescription *> *getPrescriptionList()
    {
        // Create a new vector to store prescriptions with the size equal to the number of prescriptions
        std::vector<Prescription *> *prescriptionList = new std::vector<Prescription *>(prescriptions.size());
        // Loop through prescriptions map and add pointers to the vector
        for (auto it = prescriptions.begin(); it != prescriptions.end(); ++it)
        {
            prescriptionList->push_back(it->second);
        }
        // Return the vector
        return prescriptionList;
    }

    // Destructor for ObjectCache
    ~ObjectCache()
    {
        // Release memory for objects in maps
        releasePatients();
        releaseDoctors();
        releasePrescriptions();

        // Call de_alloc() function with "ObjectCache" as an argument
        de_alloc("ObjectCache");
    }

private:
    // Method to release memory for patients
    void releasePatients()
    {
        // Get a list of patients
        std::vector<Patient *> *dataObjects = this->getPatientList();
        // Loop through the list and delete objects
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        // Delete the list itself
        delete dataObjects;
        // Set dataObjects to nullptr to avoid dangling pointers
        dataObjects = nullptr;
    }
    
    // Similar methods for doctors and prescriptions
    void releaseDoctors()
    {
        // Get a list of doctors
        std::vector<Doctor *> *dataObjects = this->getDoctorList();
        // Loop through the list and delete objects
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        // Delete the list itself
        delete dataObjects;
        // Set dataObjects to nullptr to avoid dangling pointers
        dataObjects = nullptr;
    }

    void releasePrescriptions()
    {
        // Get a list of prescriptions
        std::vector<Prescription *> *dataObjects = this->getPrescriptionList();
        // Loop through the list and delete objects
        for (int i = 0; i < dataObjects->size(); ++i)
        {
            delete (*dataObjects)[i];
        }
        // Delete the list itself
        delete dataObjects;
        // Set dataObjects to nullptr to avoid dangling pointers
        dataObjects = nullptr;
    }
};


// Initialize the static instance of ObjectCache
ObjectCache *ObjectCache::instance;


// Define a class PrescriptionBuilder that inherits from the Builder class
class PrescriptionBuilder : public Builder {
public:
    // Constructor for PrescriptionBuilder
    PrescriptionBuilder() {
        new_alloc("PrescriptionBuilder"); // Increment the count of PrescriptionBuilder objects in the ObjectCounter
        filepath += "prescription.txt"; // Update the file path for prescription data
    }

    // Method to construct DataObject pointers from data
    vector<DataObject *> *constructDataObject(vector<vector<string> > *data) {
        // Create a vector of DataObject pointers with the size of the input data
        vector<DataObject *> *dataObjects = new vector<DataObject *>(data->size());
        for (int i = 0; i < data->size(); ++i) {
            // Extract data elements from the input data vector
            string& prescriptionID = (*data)[i][0];  
            string& patient = (*data)[i][1];        
            string& doctor = (*data)[i][2];            
            string& prescriptionDetails = (*data)[i][3];
            string& datePrescribed = (*data)[i][4];
            // Create a new Prescription object and assign it to the corresponding index in dataObjects vector
            Prescription *prescription = new Prescription(prescriptionID, patient, doctor, prescriptionDetails, datePrescribed);
            (*dataObjects)[i] = prescription;

            // Add the Prescription object to ObjectCache
            ObjectCache::getInstance()->addPrescriptions(prescription); 
        }
        // Return the vector of DataObject pointers
        return dataObjects;
    }

    // Destructor for PrescriptionBuilder
    ~PrescriptionBuilder() {
        de_alloc("PrescriptionBuilder"); // Decrement the count of PrescriptionBuilder objects in the ObjectCounter
    }
};

// Define a class BuilderFactory responsible for creating different types of builders
class BuilderFactory {
private:
    // Map to store different types of builders
    map<string, Builder *> builderMap; 
    // Singleton instance of the BuilderFactory class
    static BuilderFactory *instance; 

    // Private constructor to initialize builderMap with different builders
    BuilderFactory() {
        // Initialize builderMap with instances of different builder classes
        builderMap["Patient"] = new PatientBuilder;
        builderMap["Doctor"] = new DoctorBuilder;
        builderMap["Prescription"] = new PrescriptionBuilder; 
    }

public:
    // Static method to get the singleton instance of BuilderFactory
    static BuilderFactory* getInstance() {
        // Create a new instance if instance is nullptr (lazy initialization)
        if (instance == nullptr)
            instance = new BuilderFactory; 
        return instance; // Return the singleton instance
    }

    // Method to create a specific builder based on the class name
    Builder *createBuilder(string className) {
        return builderMap[className]; // Return the corresponding builder from the builderMap
    }

    // Destructor for BuilderFactory
    ~BuilderFactory() {
        de_alloc("BuilderFactory"); // Decrement the count of BuilderFactory objects in the ObjectCounter
    }
};

// Initialize the static instance variable outside the class definition
BuilderFactory *BuilderFactory::instance = nullptr;
