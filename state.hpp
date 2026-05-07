#pragma once

class App;

class State 
{
public:
    virtual ~State() = default;

    virtual void update(App* app) = 0;
    virtual void render(App* app) = 0;    
};