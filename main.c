
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARIABLES 50
#define MAX_OBJECTS 50
#define MAX_BINDINGS 50

/* C variable record */
typedef struct {
    char name[30];
    char type[15];
    char value[50];
    char scope[20];
    char storage[20];
    int active;
} CVariable;

/* Python object record */
typedef struct {
    int objectID;
    char type[15];
    char value[50];
    int active;
} PyObject;

/* Python name-to-object binding */
typedef struct {
    char name[30];
    int objectID;
    char scope[20];
    int active;
} PyBinding;

CVariable cVariables[MAX_VARIABLES];
PyObject pyObjects[MAX_OBJECTS];
PyBinding pyBindings[MAX_BINDINGS];

int cVariableCount = 0;
int pyObjectCount = 0;
int pyBindingCount = 0;

/* Function declarations */
void mainMenu(void);
void variableBindingModule(void);
void variableLifetimeModule(void);
void comparisonModule(void);

void createVariable(void);
void displayVariables(void);
void reassignVariable(void);
void demonstrateTypeChange(void);
void demonstrateLifetime(void);

void readLine(char text[], int size);
int readChoice(void);
int validateCValue(char type[], char value[]);
int detectPythonType(char value[]);
int findCVariable(char name[]);
int findPyBinding(char name[]);
int findPyObject(int objectID);

void clearInput(void);

/* Clears remaining input */
void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* Reads a menu choice */
int readChoice(void) {
    int choice;

    if (scanf("%d", &choice) != 1) {
        clearInput();
        return -1;
    }

    clearInput();
    return choice;
}

/* Reads text input safely */
void readLine(char text[], int size) {
    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    }
}

/* Finds a C variable */
int findCVariable(char name[]) {
    int i;

    for (i = 0; i < cVariableCount; i++) {
        if (cVariables[i].active &&
            strcmp(cVariables[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

/* Finds a Python binding */
int findPyBinding(char name[]) {
    int i;

    for (i = 0; i < pyBindingCount; i++) {
        if (pyBindings[i].active &&
            strcmp(pyBindings[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

/* Finds a Python object */
int findPyObject(int objectID) {
    int i;

    for (i = 0; i < pyObjectCount; i++) {
        if (pyObjects[i].objectID == objectID &&
            pyObjects[i].active) {
            return i;
        }
    }

    return -1;
}

/* Basic C value validation */
int validateCValue(char type[], char value[]) {
    char *end;
    long intValue;
    double doubleValue;

    if (strcmp(type, "int") == 0) {
        intValue = strtol(value, &end, 10);
        return value[0] != '\0' && *end == '\0';
    }

    if (strcmp(type, "float") == 0 ||
        strcmp(type, "double") == 0) {
        doubleValue = strtod(value, &end);
        return value[0] != '\0' && *end == '\0';
    }

    if (strcmp(type, "char") == 0) {
        return strlen(value) == 1;
    }

    if (strcmp(type, "string") == 0) {
        return strlen(value) > 0;
    }

    return 0;
}

/* Simple Python type detection */
int detectPythonType(char value[]) {
    char *end;
    int hasDecimal = 0;
    int i;

    if (strcmp(value, "True") == 0 ||
        strcmp(value, "False") == 0) {
        return 3;
    }

    if (value[0] == '\0') {
        return 4;
    }

    strtod(value, &end);

    if (*end == '\0') {
        for (i = 0; value[i] != '\0'; i++) {
            if (value[i] == '.' ||
                value[i] == 'e' ||
                value[i] == 'E') {
                hasDecimal = 1;
            }
        }

        return hasDecimal ? 2 : 1;
    }

    return 4;
}

/* Converts Python type code to text */
void getPythonTypeName(int type, char result[]) {
    switch (type) {
        case 1:
            strcpy(result, "int");
            break;
        case 2:
            strcpy(result, "float");
            break;
        case 3:
            strcpy(result, "bool");
            break;
        default:
            strcpy(result, "str");
    }
}

/* Creates a variable in either language */
void createVariable(void) {
    int language;
    int typeChoice;
    int detectedType;
    char name[30];
    char type[15];
    char value[50];

    printf("\nCREATE VARIABLE\n");
    printf("1. C variable\n");
    printf("2. Python variable\n");
    printf("Choose language: ");

    language = readChoice();

    if (language != 1 && language != 2) {
        printf("Invalid choice.\n");
        return;
    }

    printf("Enter variable name: ");
    readLine(name, sizeof(name));

    if (strlen(name) == 0) {
        printf("Variable name cannot be empty.\n");
        return;
    }

    if (language == 1) {
        if (cVariableCount >= MAX_VARIABLES) {
            printf("C variable storage is full.\n");
            return;
        }

        if (findCVariable(name) != -1) {
            printf("A C variable with that name already exists.\n");
            return;
        }

        printf("\nSelect C data type:\n");
        printf("1. int\n");
        printf("2. float\n");
        printf("3. double\n");
        printf("4. char\n");
        printf("5. string (simulated)\n");
        printf("Choose type: ");

        typeChoice = readChoice();

        switch (typeChoice) {
            case 1: strcpy(type, "int"); break;
            case 2: strcpy(type, "float"); break;
            case 3: strcpy(type, "double"); break;
            case 4: strcpy(type, "char"); break;
            case 5: strcpy(type, "string"); break;
            default:
                printf("Invalid data type.\n");
                return;
        }

        printf("Enter value: ");
        readLine(value, sizeof(value));

        if (!validateCValue(type, value)) {
            printf("Invalid value for the selected data type.\n");
            return;
        }

        strcpy(cVariables[cVariableCount].name, name);
        strcpy(cVariables[cVariableCount].type, type);
        strcpy(cVariables[cVariableCount].value, value);
        strcpy(cVariables[cVariableCount].scope, "Simulated");
        strcpy(cVariables[cVariableCount].storage, "Unspecified");
        cVariables[cVariableCount].active = 1;

        cVariableCount++;

        printf("\nC variable created successfully.\n");
        printf("Name: %s\n", name);
        printf("Type: %s\n", type);
        printf("Value: %s\n", value);
        printf("Note: Storage duration has not yet been modelled.\n");
    }

    else {
        if (pyBindingCount >= MAX_BINDINGS ||
            pyObjectCount >= MAX_OBJECTS) {
            printf("Python simulation storage is full.\n");
            return;
        }

        if (findPyBinding(name) != -1) {
            printf("A Python binding with that name already exists.\n");
            return;
        }

        printf("Enter value: ");
        readLine(value, sizeof(value));

        detectedType = detectPythonType(value);
        getPythonTypeName(detectedType, type);

        pyObjects[pyObjectCount].objectID = pyObjectCount + 1;
        strcpy(pyObjects[pyObjectCount].type, type);
        strcpy(pyObjects[pyObjectCount].value, value);
        pyObjects[pyObjectCount].active = 1;

        strcpy(pyBindings[pyBindingCount].name, name);
        pyBindings[pyBindingCount].objectID =
            pyObjects[pyObjectCount].objectID;
        strcpy(pyBindings[pyBindingCount].scope, "Simulated");
        pyBindings[pyBindingCount].active = 1;

        pyObjectCount++;
        pyBindingCount++;

        printf("\nPython variable binding created successfully.\n");
        printf("Name: %s\n", name);
        printf("Detected type: %s\n", type);
        printf("Value: %s\n", value);
        printf("Object ID: %d\n", pyObjectCount);
    }
}

/* Displays all active variables */
void displayVariables(void) {
    int i;
    int objectIndex;

    printf("\n========== C VARIABLES ==========\n");

    for (i = 0; i < cVariableCount; i++) {
        if (cVariables[i].active) {
            printf("\nName: %s", cVariables[i].name);
            printf("\nType: %s", cVariables[i].type);
            printf("\nValue: %s", cVariables[i].value);
            printf("\nScope: %s", cVariables[i].scope);
            printf("\nStorage: %s\n", cVariables[i].storage);
        }
    }

    printf("\n========== PYTHON BINDINGS ==========\n");

    for (i = 0; i < pyBindingCount; i++) {
        if (pyBindings[i].active) {
            objectIndex = findPyObject(
                pyBindings[i].objectID
            );

            if (objectIndex != -1) {
                printf("\nName: %s", pyBindings[i].name);
                printf("\nObject ID: %d",
                       pyBindings[i].objectID);
                printf("\nType: %s",
                       pyObjects[objectIndex].type);
                printf("\nValue: %s",
                       pyObjects[objectIndex].value);
                printf("\nScope: %s\n",
                       pyBindings[i].scope);
            }
        }
    }
}

/* Reassigns a variable */
void reassignVariable(void) {
    int language;
    int index;
    int objectIndex;
    char name[30];
    char value[50];
    char type[15];
    int detectedType;

    printf("\nREASSIGN VARIABLE\n");
    printf("1. C\n");
    printf("2. Python\n");
    printf("Choose language: ");

    language = readChoice();

    printf("Enter variable name: ");
    readLine(name, sizeof(name));

    if (language == 1) {
        index = findCVariable(name);

        if (index == -1) {
            printf("C variable not found.\n");
            return;
        }

        printf("Existing type: %s\n",
               cVariables[index].type);

        printf("Enter new value: ");
        readLine(value, sizeof(value));

        if (!validateCValue(
                cVariables[index].type, value)) {
            printf("Invalid value. C variable type cannot change.\n");
            return;
        }

        strcpy(cVariables[index].value, value);

        printf("C variable reassigned successfully.\n");
        printf("Type remains: %s\n",
               cVariables[index].type);
    }

    else if (language == 2) {
        index = findPyBinding(name);

        if (index == -1) {
            printf("Python binding not found.\n");
            return;
        }

        if (pyObjectCount >= MAX_OBJECTS) {
            printf("Python object storage is full.\n");
            return;
        }

        printf("Enter new value: ");
        readLine(value, sizeof(value));

        detectedType = detectPythonType(value);
        getPythonTypeName(detectedType, type);

        /* Create a new object and rebind the name */
        pyObjects[pyObjectCount].objectID = pyObjectCount + 1;
        strcpy(pyObjects[pyObjectCount].type, type);
        strcpy(pyObjects[pyObjectCount].value, value);
        pyObjects[pyObjectCount].active = 1;

        pyBindings[index].objectID =
            pyObjects[pyObjectCount].objectID;

        pyObjectCount++;

        printf("Python name rebound successfully.\n");
        printf("New type: %s\n", type);
        printf("New value: %s\n", value);
        printf("A new object was created in the simulation.\n");
    }

    else {
        printf("Invalid language choice.\n");
    }
}

/* Demonstrates Python's dynamic type behaviour */
void demonstrateTypeChange(void) {
    char name[30];
    int index;
    int objectIndex;

    printf("\nPYTHON TYPE CHANGE DEMONSTRATION\n");
    printf("Enter Python variable name: ");
    readLine(name, sizeof(name));

    index = findPyBinding(name);

    if (index == -1) {
        printf("Python binding not found.\n");
        return;
    }

    objectIndex = findPyObject(pyBindings[index].objectID);

    if (objectIndex == -1) {
        printf("Object not found.\n");
        return;
    }

    printf("\nCurrent binding:\n");
    printf("Name: %s\n", name);
    printf("Type: %s\n", pyObjects[objectIndex].type);
    printf("Value: %s\n", pyObjects[objectIndex].value);

    printf("\nPython allows a name to be rebound to an object ");
    printf("of a different type.\n");
    printf("Use the reassignment option to demonstrate this.\n");
}

/* Demonstrates variable lifetime and scope */
void demonstrateLifetime(void) {
    int choice;

    printf("\nVARIABLE LIFETIME DEMONSTRATION\n");
    printf("1. C automatic local variable\n");
    printf("2. C static local variable\n");
    printf("3. C dynamically allocated memory\n");
    printf("4. Python name binding and object lifetime\n");
    printf("Choose demonstration: ");

    choice = readChoice();

    switch (choice) {
        case 1: {
            int localVariable = 25;

            printf("\nA local C variable has been created.\n");
            printf("Value: %d\n", localVariable);
            printf("It has automatic storage duration.\n");
            printf("Its lifetime ends when its block is exited.\n");
            break;
        }

        case 2: {
            static int staticVariable = 10;

            printf("\nStatic variable value: %d\n",
                   staticVariable);

            staticVariable++;

            printf("Value after increment: %d\n",
                   staticVariable);

            printf("A static local variable retains its value ");
            printf("between function calls.\n");
            printf("It has static storage duration.\n");
            break;
        }

        case 3: {
            int *dynamicVariable;

            dynamicVariable = (int *)malloc(sizeof(int));

            if (dynamicVariable == NULL) {
                printf("Memory allocation failed.\n");
                break;
            }

            *dynamicVariable = 100;

            printf("\nDynamic memory allocated successfully.\n");
            printf("Stored value: %d\n", *dynamicVariable);

            free(dynamicVariable);
            dynamicVariable = NULL;

            printf("Memory released using free().\n");
            break;
        }

        case 4: {
            int index;
            int objectIndex;
            int i;
            int referenceCount;

            char name[30];

            printf("\nEnter Python variable name: ");
            readLine(name, sizeof(name));

            index = findPyBinding(name);

            if (index == -1) {
                printf("Python binding not found.\n");
                break;
            }

            objectIndex = findPyObject(
                pyBindings[index].objectID
            );

            if (objectIndex == -1) {
                printf("Object not found.\n");
                break;
            }

            referenceCount = 0;

            for (i = 0; i < pyBindingCount; i++) {
                if (pyBindings[i].active &&
                    pyBindings[i].objectID ==
                    pyBindings[index].objectID) {
                    referenceCount++;
                }
            }

            printf("\nVariable name: %s\n", name);
            printf("Object ID: %d\n",
                   pyBindings[index].objectID);
            printf("Current references: %d\n",
                   referenceCount);

            printf("\nRemoving the name binding...\n");
            pyBindings[index].active = 0;

            referenceCount--;

            printf("Remaining references: %d\n",
                   referenceCount);

            if (referenceCount == 0) {
                printf("No active name in this simulation ");
                printf("refers to the object.\n");
                printf("The object may become eligible for ");
                printf("garbage collection.\n");
            } else {
                printf("The object remains referenced by ");
                printf("another name.\n");
            }

            printf("Actual Python object destruction depends ");
            printf("on the implementation and garbage collector.\n");
            break;
        }

        default:
            printf("Invalid choice.\n");
    }
}

/* Compares C and Python */
void comparisonModule(void) {
    printf("\n====================================\n");
    printf(" C AND PYTHON: BINDING AND LIFETIME\n");
    printf("====================================\n");

    printf("\n1. VARIABLE DECLARATION\n");
    printf("C: Variables are declared with a specified type.\n");
    printf("Python: Names are bound to objects at runtime.\n");

    printf("\n2. TYPE REASSIGNMENT\n");
    printf("C: A variable's declared type remains fixed.\n");
    printf("Python: A name can be rebound to an object ");
    printf("of another type.\n");

    printf("\n3. VARIABLE LIFETIME\n");
    printf("C: Lifetime depends on storage duration.\n");
    printf("Python: Names and objects have distinct lifetimes.\n");

    printf("\n4. MEMORY MANAGEMENT\n");
    printf("C: Dynamically allocated memory is manually managed ");
    printf("using malloc() and free().\n");
    printf("Python: Memory management is handled automatically, ");
    printf("with implementation-dependent garbage collection.\n");

    printf("\n5. BINDING\n");
    printf("C: A declared variable is associated with a typed ");
    printf("object or storage location.\n");
    printf("Python: A name refers to an object through a binding.\n");
}

/* Variable binding menu */
void variableBindingModule(void) {
    int choice;

    do {
        printf("\n========== VARIABLE BINDING ==========\n");
        printf("1. Create variable\n");
        printf("2. Display variables\n");
        printf("3. Reassign variable\n");
        printf("4. Demonstrate Python type change\n");
        printf("0. Return to main menu\n");
        printf("Choose option: ");

        choice = readChoice();

        switch (choice) {
            case 1:
                createVariable();
                break;

            case 2:
                displayVariables();
                break;

            case 3:
                reassignVariable();
                break;

            case 4:
                demonstrateTypeChange();
                break;

            case 0:
                break;

            default:
                printf("Invalid option.\n");
        }

    } while (choice != 0);
}

/* Variable lifetime menu */
void variableLifetimeModule(void) {
    int choice;

    do {
        printf("\n========== VARIABLE LIFETIME ==========\n");
        printf("1. Run lifetime demonstration\n");
        printf("0. Return to main menu\n");
        printf("Choose option: ");

        choice = readChoice();

        switch (choice) {
            case 1:
                demonstrateLifetime();
                break;

            case 0:
                break;

            default:
                printf("Invalid option.\n");
        }

    } while (choice != 0);
}

/* Main menu */
void mainMenu(void) {
    int choice;

    do {
        printf("\n");
        printf("============================================\n");
        printf(" VARIABLE BINDING AND LIFETIME SIMULATOR\n");
        printf("============================================\n");
        printf("1. Variable Binding Module\n");
        printf("2. Variable Lifetime Module\n");
        printf("3. C and Python Comparison\n");
        printf("0. Exit Program\n");
        printf("--------------------------------------------\n");
        printf("Choose an option: ");

        choice = readChoice();

        switch (choice) {
            case 1:
                variableBindingModule();
                break;

            case 2:
                variableLifetimeModule();
                break;

            case 3:
                comparisonModule();
                break;

            case 0:
                printf("\nThank you for using the simulator.\n");
                break;

            default:
                printf("Invalid option. Try again.\n");
        }

    } while (choice != 0);
}

/* Program entry point */
int main(void) {
    mainMenu();
    return 0;
}
