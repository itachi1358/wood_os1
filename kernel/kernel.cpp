
extern "C" void kernel_main();

#include <stdint.h> // add this if not already included
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
class Keyboard {
private:
    static const char scancode_to_ascii[128];

public:
    char get_key() {
        int scancode;
        while (!(inb(0x64) & 1)); // Wait for key press
        scancode = inb(0x60);
        if (scancode & 0x80) return 0; // key release
        return scancode_to_ascii[scancode];
    }
};

const char Keyboard::scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    // Fill rest with 0
};

char* video = (char*)0xB8000;
int cursor = 0;

void print_char(char c) {
    if (c == '\n') {
        cursor += (160 - cursor % 160); // move to new line
    } else {
        video[cursor++] = c;
        video[cursor++] = 0x07;
    }
}

void print_string(const char* s) {
    while (*s) print_char(*s++);
}

void clear_screen() {
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
    cursor =  0;
}

void print_prompt() {
    print_string("woodOS> ");
}

bool strcmp(const char* str1,const char* str2){

     while (*str1 && *str2){
        if (*str1 != *str2) return false;
        str1++;
        str2++;
     }
     return *str1 == '\0' && *str2 == '\0';
}
char* split_exp( char*str){
    char* ans;
    while(*str){
        if(*str == ' '){
            ans = str+1;
            break;
        }
        str++;
    }
    return ans;
}

class folder{
    char notepad_files[5][180][180];
    //only 5 files can be stored

};

static inline void outw(unsigned short port, unsigned short val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}
void shutdown() {
    outw(0x604, 0x2000);  // This tells QEMU to power off (Bochs/QEMU specific)
}



// char notepad_file[180][180];
int row = 0, col = 0;
bool overwrite_mode = true; // starts in overwrite mode

void render_buffer(char notepad_file[180][180], bool should_clear){
    if (should_clear) clear_screen();
    
    for (int i = 0; i < 180; i++) {
        for (int j = 0; j < 180; j++) {
            if (notepad_file[i][j] == '\0') break;
            print_char(notepad_file[i][j]);
        }
        print_char('\n');
    }

    // Set cursor based on row and col
    cursor = (row * 160) + (col * 2);
}

class notepad {
public:
    Keyboard keyboard;

    void open_file(char notepad_file[180][180]) {
        bool is_it_done = false;
        render_buffer(notepad_file,1);
        print_string("\nYou are in Notepad. Press [ to exit. Use Tab to toggle Overwrite/Insert mode.\n");

        while (!is_it_done) {
            char c = keyboard.get_key();
            if (c) {
                if (c == '[') {
                    print_string("\nSave changes?\n1. Yes\n2. No\n");
                    while (1) {
                        char choice = keyboard.get_key();
                        if (choice == '1') {
                            print_string("File saved.\n");
                            is_it_done = true;
                            break;
                        } else if (choice == '2') {
                            for (int i = 0; i < 180; i++) {
                                for (int j = 0; j < 180; j++) {
                                    notepad_file[i][j] = '\0';
                                }
                            }
                            print_string("File discarded.\n");
                            is_it_done = true;
                            break;
                        }
                    }
                }

                else if (c == '\b') {
                    if (col > 0) {
                        col--;
                        for (int j = col; j < 179; j++) {
                            notepad_file[row][j] = notepad_file[row][j + 1];
                        }
                        notepad_file[row][179] = '\0';
                    } else if (row > 0) {
                        row--;
                        col = sizeof(notepad_file[row])/4;
                    }
                    render_buffer(notepad_file,1);
                }

                else if (c == '\n') {
                    if (row < 179) {
                        row++;
                        col = 0;
                    }
                    render_buffer(notepad_file,1);
                }

                else {
                    if (overwrite_mode) {
                        // Overwrite character
                        notepad_file[row][col] = c;
                    } else {
                        // Insert character by shifting to the right
                        for (int j = 179; j > col; j--) {
                            notepad_file[row][j] = notepad_file[row][j - 1];
                        }
                        notepad_file[row][col] = c;
                    }
                    if (col < 179) col++;
                    render_buffer(notepad_file,1);
                }
            }
        }
    }
};


 char notepad_files[6][180][180];
    //only 6 files can be stored
    char User[20]="abc";
char Pass[20]="123";
void process_command( char* cmd) {
    if (strcmp(cmd, "hello")) {
        print_string("Hello from woodOS!\n");
    } else if (strcmp(cmd, "clear")) {
        clear_screen();
    }
    else if(cmd[0]=='c' && cmd[1]=='a' && cmd[2]=='l' && cmd[3]=='c'){
        char *exp = split_exp(cmd);
        print_string(exp);
        print_char('\n');
        print_string("calculation\n");
    } 
    else if(cmd[0]=='w' && cmd[1]=='r' && cmd[2]=='i' && cmd[3]=='t' && cmd[4]=='e'){
        int file_idx=cmd[6]-'0';
        notepad np;
        np.open_file(notepad_files[file_idx]);
    }
    else if(strcmp(cmd, "ltf")){
        print_string("Files:\n");
        bool valid_files[6];
        for(int i=0;i<6;i++){
            valid_files[i]=false; 
        }
        for(int i=0;i<5;i++){
            for(int j=0;j<180;j++){
                for(int k=0;k<180;k++){
                    if(notepad_files[i][j][k]!='\0'){
                        valid_files[i]=true;
                    }
                }
            }
        }
        for(int i=0;i<6;i++){
            if(valid_files[i]){
                char c='0'+i;
                print_char(c);
                print_char('\n');
            }
        }
    }
    else if(cmd[0]=='s' && cmd[1]=='h' && cmd[2]=='o' && cmd[3]=='w'){
        int file_idx=cmd[5]-'0';
        bool valid=false;
            for(int j=0;j<180;j++){
                for(int k=0;k<180;k++){
                    if(notepad_files[file_idx][j][k]!='\0'){
                        valid=true;
                        break;
                    }
                }
            }
            if(!valid){
                print_string("File not found\n");
            }
            else{
                 print_char('\n');
                render_buffer(notepad_files[file_idx],0);
                print_char('\n');
            }
    }
    else if(cmd[0]=='t' && cmd[1]=='o' && cmd[2]=='u' && cmd[3]=='c' && cmd[4]=='h'){
        int file_idx=cmd[6]-'0';
         bool valid=false;
            for(int j=0;j<180;j++){
                for(int k=0;k<180;k++){
                    if(notepad_files[file_idx][j][k]!='\0'){
                        valid=true;
                        break;
                    }
                }
            }
            if(!valid){
               notepad_files[file_idx][0][0]=' ';
            }
            else{
                print_string("Cannot Create a File::File already exists");
            }
    }
    else if(cmd[0]=='d' && cmd[1]=='e' && cmd[2]=='l'){
        int file_idx=cmd[4]-'0';
        for(int i=0;i<180;i++){
            for(int j=0;j<180;j++){
                notepad_files[file_idx][i][j]='\0';
            }
        }
    }
    else if(strcmp(cmd, "uchange")){
        print_string("\nWhat's the new Username?:");
        char new_username[20];
        int idx=0;
        Keyboard keyboad;
        while(1){
            char c=keyboad.get_key();
            if(c){
                if(c=='\n'){
                    new_username[idx]='\0';
                    for(int i=0;i<=idx;i++){
                        User[i]=new_username[i];
                    }
                }
                else if(c=='\b'){
                    if(idx>0){
                    idx--;
                    cursor-=2;
                    print_char(' ');
                    cursor-=2;
                    }
                }
                else{
                    if (idx < 24) {  // Prevent overflow
                        new_username[idx++] = c;
                        print_char(c);
                    }
                }
            }
        }
    }
    else if (strcmp(cmd, "shutdown")) {
        print_string("Shutting down...\n");
        shutdown();
    }
    else if (strcmp(cmd, "help")) {
        print_string("Available Commands:\n");
        print_string("  hello        - Prints a welcome message from woodOS\n");
        print_string("  clear        - Clears the screen\n");
        print_string("  calc <expr>  - Calculates the expression (planned feature)\n");
        print_string("  write <n>    - Opens file number <n> (0-5) in notepad mode\n");
        print_string("  touch <n>    - Creates an empty file at index <n> (0-5)\n");
        print_string("  del <n>      - Deletes the file at index <n> (0-5)\n");
        print_string("  show <n>     - Displays the contents of file at index <n> (0-5)\n");
        print_string("  ltf          - Lists all valid files (0-5) with contents\n");
        print_string("  uchange      - Change the username\n");
        print_string("  shutdown     - Shuts down the OS\n");
        print_string("  help         - Displays this help message\n");
    }
    else {
        print_string("Unknown command\n");
    }
}

extern "C" void kernel_main() {
    Keyboard keyboard;
    char command[100];
    int index = 0;
    char username[100],password[100];
    int uid=0,pid=0;
    clear_screen();
    print_string("Hello User Please Enter You Username:\n");
    bool username_crct=false;
    while (1) {
        char c1 = keyboard.get_key();
        if(c1){
        if (c1 == '\n') {
            username[uid] = '\0';  // Null-terminate
            if (strcmp(username, User)) {
                username_crct = true;
                break;
            } else {
                print_string("\nInvalid Username, Try Again\n");
                uid = 0;
            }
        } else if (c1 == '\b') {
            if (uid > 0) {
                uid--;
                cursor -= 2;
                print_char(' ');
                cursor -= 2;
            }
        } else {
            if (uid < 19) {  // Reserve space for '\0'
                username[uid] = c1;
                uid++;
                print_char(c1);
            }
        }
    }
}

   
    print_string("Please Enter Your Password:\n");
    bool password_crct=false;
    while(1 && !password_crct){
        char c=keyboard.get_key();
        if(c){
            if(c=='\n'){
                if(strcmp(password,Pass)){
                    password_crct=1;
                    break;
                }
                else{
                    print_string("Invalid Password, Try Again \n");
                }
            }
            else if(c=='\b'){
                if(pid>0){
                pid--;
                cursor-=2;
                print_char(' ');
                cursor-=2;
                }
            }
            else{
                if (uid < 24) {  // Prevent overflow
                    password[pid++] = c;
                    print_char(c);
                }
            }
        }
    }

    clear_screen();
      print_prompt();
  while (1) {
    char c = keyboard.get_key();
    if (c) {
        if (c == '\n') {  // ENTER key
            command[index] = '\0';
             print_char('\n');

            process_command(command);
            print_prompt();
            index = 0;
        } else if (c == '\b') {  // BACKSPACE
            if (index > 0) {
                index--;
                cursor -= 2;
                print_char(' ');
                cursor -= 2;
            }
        } else {
            if (index < 127) {  // Prevent overflow
                command[index++] = c;
                print_char(c);
            }
        }
    }
}

}
