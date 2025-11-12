#include <stdio.h>
#include <string.h>

// Define roles
typedef enum {
    ROLE_VIEWER,
    ROLE_EDITOR,
    ROLE_ADMIN
} Role;

// Define user structure
typedef struct {
    char username[20];
    char password[20];
    Role role;
} User;

// Predefined users
User users[] = {
    {"alice", "1234", ROLE_VIEWER},
    {"bob", "abcd", ROLE_EDITOR},
    {"admin", "admin", ROLE_ADMIN}
};

int total_users = 3;

// Function to get role name
const char* getRoleName(Role role) {
    switch (role) {
        case ROLE_VIEWER: return "Viewer";
        case ROLE_EDITOR: return "Editor";
        case ROLE_ADMIN:  return "Admin";
        default: return "Unknown";
    }
}

// Check credentials
User* authenticate(char* username, char* password) {
    for (int i = 0; i < total_users; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

// Actions
void viewRecords() {
    printf("✅ Viewing records...\n");
}

void editRecords() {
    printf("✏️  Editing records...\n");
}

void deleteRecords() {
    printf("🗑️  Deleting records...\n");
}

int main() {
    char username[20], password[20];
    printf("=== Simple RBAC System ===\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    User* user = authenticate(username, password);
    if (!user) {
        printf("❌ Invalid credentials.\n");
        return 1;
    }

    printf("\nWelcome, %s! Your role: %s\n\n", user->username, getRoleName(user->role));

    int choice;
    while (1) {
        printf("Choose an action:\n");
        printf("1. View records\n");
        printf("2. Edit records\n");
        printf("3. Delete records\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewRecords();
                break;
            case 2:
                if (user->role == ROLE_EDITOR || user->role == ROLE_ADMIN)
                    editRecords();
                else
                    printf("🚫 Access denied: Editors or Admins only.\n");
                break;
            case 3:
                if (user->role == ROLE_ADMIN)
                    deleteRecords();
                else
                    printf("🚫 Access denied: Admins only.\n");
                break;
            case 4:
                printf("👋 Logged out. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
        printf("\n");
    }

    return 0;
}

