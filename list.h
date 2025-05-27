#ifndef LIST_H
#define LIST_H

enum BlockType { LETTERS, SPACES, COMMA, PUNCTUATION };

struct Letters {
    char data[6];
    int size;
    bool is_word_part;
};

struct Spaces {
    int count;
};

struct Comma {
    char comma;
};

struct Punctuation {
    char symbol;
};

struct EL_Stroka {
    BlockType type;
    union {
        Letters letters;
        Spaces spaces;
        Comma comma;
        Punctuation punctuation;
    } content;
    int sentence_id;
    EL_Stroka* next;
    EL_Stroka* next_word_block;
};

struct Line {
    EL_Stroka* head;
};

struct EL_V {
    Line line;
    EL_V* next;
};

struct Form_V {
    EL_V* head;
    int total_sentences;
};

void InitDocument(Form_V& doc);
void InitLevel(EL_V* level);
void AddLevelToDoc(Form_V& doc, EL_V* level);
void InitBlock(EL_Stroka*& block, BlockType type);
void AddBlockToLine(Line& line, EL_Stroka* block);

#endif