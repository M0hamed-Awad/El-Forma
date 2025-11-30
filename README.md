# El-Forma Gym Management System - Architecture Documentation

## Project Overview

El-Forma is a gym management system built in C++ that manages members, trainers, and administrators with an in-memory data storage system.

---

## Use Cases

### Main Usecase
![main](https://github.com/user-attachments/assets/66a12a09-b817-4d66-a8b9-14217cbb9394)


### Refactor Usecase
![refactor](https://github.com/user-attachments/assets/06415e72-3e2a-4480-a1b3-a49b17b2069e)

## Activity Diagrams

<table>
    <tr>
      <td>
        <h4>Add Member</h4>
        <img src="https://github.com/user-attachments/assets/48769c20-46b6-4d0c-8f83-fe3cb67a84a9" alt="Add Member" />
      </td>
      <td>
        <h4>Update Member</h4>
        <img src="https://github.com/user-attachments/assets/ab9e5790-7282-4b36-874a-923dc4a808b7" alt="Update Member" />
      </td>
      <td>
        <h4>Assign Member to Trainer</h4>
        <img src="https://github.com/user-attachments/assets/39b9eae7-1c58-43a2-baf8-905a00e575b7" alt="Assign Member to Trainer" />
      </td>
    </tr>
  </table>


---

## Project Structure

```
El-Forma/
├── src/
│   ├── main.cpp                    # Entry point
│   ├── entities/                   # Domain entities
│   │   ├── User.h                  # Base user class
│   │   ├── Admin.h                 # Administrator entity
│   │   ├── Member.h                # Member entity
│   │   ├── Trainer.h               # Trainer entity
│   │   └── System.h                # Main system controller
│   ├── services/                   # Service layer (Business logic + UI)
│   │   ├── ConsoleUI.h             # Console UI utilities
│   │   ├── AdminService.h          # Admin operations & UI
│   │   ├── MemberService.h         # Member operations & UI
│   │   └── TrainerService.h        # Trainer operations & UI
│   └── output/                     # Compiled executables
```

---

## Architecture Overview

```mermaid
graph TB
    Main[main.cpp] --> System[System.h]
    System --> AdminService[AdminService]
    System --> MemberService[MemberService]
    System --> TrainerService[TrainerService]
    
    AdminService --> Admin[Admin Entity]
    MemberService --> Member[Member Entity]
    TrainerService --> Trainer[Trainer Entity]
    
    AdminService --> ConsoleUI[ConsoleUI]
    MemberService --> ConsoleUI
    TrainerService --> ConsoleUI
    
    Admin --> User[User Base Class]
    Member --> User
    Trainer --> User
    
    style System fill:#e1f5ff
    style AdminService fill:#fff4e1
    style MemberService fill:#fff4e1
    style TrainerService fill:#fff4e1
    style ConsoleUI fill:#e8f5e9
```

---

## Class Hierarchy

### Entity Inheritance

```mermaid
classDiagram
    User <|-- Admin
    User <|-- Member
    User <|-- Trainer
    
    class User {
        -int id
        -string name
        -string email
        -string password
        +User(name, email, password)
        +getId() int
        +getName() string
        +getEmail() string
    }
    
    class Admin {
        +Admin(name, email, password)
        +login(email, password) bool
        +logout() void
    }
    
    class Member {
        -string joinDate
        -int subscriptionId
        +static int nextMemberId
        +Member(name, email, password)
        +Member(name, email, password, joinDate)
        +getJoinDate() string
        +getSubscriptionId() int
        +setSubscriptionId(int)
    }
    
    class Trainer {
        -string trainerSpecialty
        -vector~Member*~ assignedMembers
        +static int nextTrainerId
        +Trainer(name, email, password, specialty)
        +getTrainerSpecialty() string
        +setTrainerSpecialty(string)
        +assignMember(Member*)
        +getAssignedMembers() vector
    }
```

---

## Service Layer Architecture

```mermaid
graph LR
    subgraph Services ["Service Layer (Business Logic + UI)"]
        AS[AdminService]
        MS[MemberService]
        TS[TrainerService]
    end
    
    subgraph Storage ["In-Memory Storage"]
        AD[(Static vector~Admin*~)]
        MD[(Static vector~Member*~)]
        TD[(Static vector~Trainer*~)]
    end
    
    subgraph UI ["User Interface"]
        CUI[ConsoleUI]
    end
    
    AS --> AD
    MS --> MD
    TS --> TD
    
    AS --> CUI
    MS --> CUI
    TS --> CUI
    
    style Services fill:#fff4e1
    style Storage fill:#e3f2fd
    style UI fill:#e8f5e9
```

---

## Service Classes Details

### AdminService

**Responsibilities:**
- Admin authentication (login/logout)
- Admin management
- UI for admin operations

**Key Methods:**
- `login()` - Handles admin login with UI
- `logout(Admin*)` - Handles admin logout with UI
- `authenticate(email, password)` - Validates credentials
- `findAdminByEmail(email)` - Finds admin by email
- `addAdmin(Admin*)` - Adds new admin

**Data:**
- Static `vector<Admin*> admins` - In-memory admin storage
- Default admin: admin / 123

---

### MemberService

**Responsibilities:**
- Member CRUD operations
- UI for member management
- Member data storage

**Key Methods:**
- `addMember()` - Add new member with UI
- `viewAllMembers()` - Display all members in table
- `updateMember()` - Update member subscription
- `deleteMember()` - Delete member with confirmation
- `findMemberById(id)` - Find member by ID
- `getAllMembers()` - Get all members (for trainer assignment)
- `isEmpty()` - Check if members exist

**Data:**
- Static `vector<Member*> members` - In-memory member storage
- Test data: Mohamed, Ahmed, Mostafa

---

### TrainerService

**Responsibilities:**
- Trainer CRUD operations
- UI for trainer management
- Member assignment to trainers
- Trainer data storage

**Key Methods:**
- `addTrainer()` - Add new trainer with UI
- `viewAllTrainers()` - Display all trainers in table
- `viewAssignedMembers()` - Show members assigned to a trainer
- `updateTrainer(availableMembers)` - Update trainer or assign members
- `deleteTrainer()` - Delete trainer with confirmation
- `findTrainerById(id)` - Find trainer by ID
- `isEmpty()` - Check if trainers exist

**Data:**
- Static `vector<Trainer*> trainers` - In-memory trainer storage
- Test data: Amir (Cardio), Kareem (Strength), Maged (Yoga)

---

## System Class (Main Controller)

```mermaid
classDiagram
    class System {
        -Admin* currentAdmin
        -AdminService adminService
        -MemberService memberService
        -TrainerService trainerService
        
        +login() bool
        +logout() void
        +isLoggedIn() bool
        +getCurrentAdmin() Admin*
        
        +addMember() void
        +viewAllMembers() void
        +updateMember() void
        +deleteMember() void
        
        +addTrainer() void
        +viewAllTrainers() void
        +viewAssignedMembers() void
        +updateTrainer() void
        +deleteTrainer() void
        
        +showAuthMenu() void
        +showMainMenu() void
        +showMembersMenu() void
        +showTrainersMenu() void
        +handleMembersMenu() void
        +handleTrainersMenu() void
        +run() void
    }
```

**Responsibilities:**
- Application entry point
- Menu navigation
- Session management (current admin)
- Delegates all operations to services

---

## Data Flow

### Member Creation Flow

```mermaid
sequenceDiagram
    participant User
    participant System
    participant MemberService
    participant Member
    participant Storage
    
    User->>System: Select "Add Member"
    System->>MemberService: addMember()
    MemberService->>User: Request member details (UI)
    User->>MemberService: Provide name, email, password
    MemberService->>Member: new Member(details)
    Member->>Member: Auto-assign ID
    MemberService->>Storage: members.push_back(newMember)
    MemberService->>User: Display success message
```

### Login Flow

```mermaid
sequenceDiagram
    participant User
    participant System
    participant AdminService
    participant Admin
    
    User->>System: Select "Login"
    System->>AdminService: login()
    AdminService->>User: Request email & password (UI)
    User->>AdminService: Provide credentials
    AdminService->>AdminService: authenticate(email, password)
    loop For each admin
        AdminService->>Admin: login(email, password)
        Admin-->>AdminService: true/false
    end
    AdminService-->>System: Admin* (or nullptr)
    System->>System: Set currentAdmin
    System->>User: Display result message
```

---

## Key Design Patterns

### 1. **Service Layer Pattern**
- Services encapsulate business logic and UI
- Clear separation: System (controller) → Services (logic + UI) → Entities (data)

### 2. **Static Storage (Singleton-like)**
- Each service maintains static storage for its entities
- Shared across all service instances
- Data persists during application session

### 3. **Inheritance Hierarchy**
- `User` base class for common user functionality
- `Admin`, `Member`, `Trainer` extend with specific features

### 4. **Delegation Pattern**
- `System` delegates all operations to appropriate services
- No business logic in System class

---

## Memory Management

### Storage Lifecycle

- **Initialization**: Services create test data on first access
- **Runtime**: Entities dynamically allocated with `new`
- **Session**: Data persists in static vectors
- **Cleanup**: Manual deletion when removing entities
- **Restart**: All data lost (in-memory only)

### Important Notes

⚠️ **No Persistence**: Data is lost when application closes  
✅ **Default Admin**: Always available (mohamed@gmail.com / admin)  
✅ **Auto-increment IDs**: Each entity type manages its own ID counter  

---

## Test Data

### Default Admin
| Name           | Email             | Password |
| -------------- | ----------------- | -------- |
| Mohamed Rashad | mohamed@gmail.com | admin    |

### Default Members
| ID  | Name    | Email             | Password | Join Date  | Subscription |
| --- | ------- | ----------------- | -------- | ---------- | ------------ |
| 1   | Mohamed | mohamed@gmail.com | 123      | 2024-01-15 | 1            |
| 2   | Ahmed   | ahmed@gmail.com   | 123      | 2024-02-20 | 2            |
| 3   | Mostafa | mostafa@gmail.com | 123      | 2024-03-10 | 0            |

### Default Trainers
| ID  | Name   | Email            | Password   | Specialty         |
| --- | ------ | ---------------- | ---------- | ----------------- |
| 1   | Amir   | amir@gmail.com   | trainer123 | Cardio            |
| 2   | Kareem | kareem@gmail.com | trainer456 | Strength Training |
| 3   | Maged  | maged@gmail.com  | trainer789 | Yoga              |

---

## Compilation & Execution

```bash
# Compile
g++ -Wall -Wextra -g3 src/main.cpp -o src/output/main.exe

# Run
cd src/output
./main.exe
```

**Compiler Warnings:** 
- Inline static variables require C++17 (`-std=c++17`)

---
