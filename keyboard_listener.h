#ifndef H_KEYBOARD_LISTENER
#define H_KEYBOARD_LISTENER



class KeyboardListener
{
public:
    virtual void onKeyPress(int key) = 0;
    virtual void onKeyRelease(int key) = 0;
    virtual void onKeyRepeat(int key) = 0;
};



#endif // H_KEYBOARD_LISTENER
