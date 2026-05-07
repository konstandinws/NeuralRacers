#pragma once

class App;

class State 
{
public:
    virtual bool isPausable() const { return false; };

    virtual ~State() = default;

    virtual void update(App* app) = 0;
    virtual void render(App* app) = 0;    
};