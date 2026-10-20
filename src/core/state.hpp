#pragma once

class App;

class State 
{
public:
    virtual bool isPausable() const { return false; };

    virtual ~State() = default;

    // called when the state is pushed on the stack (like componentDidMount in React):
    // this is where the UI is built, because the callbacks need app
    virtual void onEnter(App* app) { (void)app; }

    virtual void update(App* app) = 0;
    virtual void render(App* app) = 0;    
};