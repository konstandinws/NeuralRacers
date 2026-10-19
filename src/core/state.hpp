#pragma once

class App;

class State 
{
public:
    virtual bool isPausable() const { return false; };

    virtual ~State() = default;

    // appele quand l'etat est ajoute a la pile (comme componentDidMount en React) :
    // c'est la qu'on construit l'interface, car les callbacks ont besoin de app
    virtual void onEnter(App* app) { (void)app; }

    virtual void update(App* app) = 0;
    virtual void render(App* app) = 0;    
};