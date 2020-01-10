/*
#include <iostream>
#include <string>
using namespace std;
*/

/////////////////////////////////////////////////////////////////
////////////// INVESTMENT IMPLEMENTATION ///////////////////
/////////////////////////////////////////////////////////////////

class Investment    // BASE CLASS
{
public:
    
    // Constructor & destructor
    Investment(string name, int price)
    {
        m_name = name;
        m_price = price;
    }
    
    virtual ~Investment()
    { }
    
    // Accessors
    string name() const
    {
        return m_name;
    }
    
    int purchasePrice() const
    {
        return m_price;
    }
    
    // Pure Virtual Functions
    virtual bool fungible() const = 0;
    virtual string description() const = 0;
    
private:
    
    string m_name;
    int m_price;
};


/////////////////////////////////////////////////////////////////
/////////////////// STOCK IMPLEMENTATION ///////////////////
/////////////////////////////////////////////////////////////////

class Stock : public Investment     // DERIVED CLASS
{
public:
    
    // Constructor & destructor
    Stock(string name, int price, string tsymbol)
    : Investment(name, price)
    {
        m_ticker = tsymbol;
    }
    
    virtual ~Stock()
    {
        cout << "Destroying " << name() << ", a stock holding" << endl;
    }
    
    // Specialized functions
    virtual bool fungible() const
    {
        return true;
    }
    
    virtual string description() const
    {
        string s = "stock trading as ";
        s += m_ticker;
        
        return s;
    }
    
private:
    
    string m_ticker;
};


/////////////////////////////////////////////////////////////////
////////////////// PAINTING IMPLEMENTATION /////////////////
/////////////////////////////////////////////////////////////////

class Painting : public Investment      // DERIVED CLASS
{
public:
    
    // Constructor & destructor
    Painting(string name, int price)
    : Investment(name, price)
    { }
    
    virtual ~Painting()
    {
        cout << "Destroying " << name() << ", a painting" << endl;
    }
    
    // Specialized Functions
    virtual bool fungible() const
    {
        return false;
    }
    
    virtual string description() const
    {
        return "painting";
    }
    
private:

};


/////////////////////////////////////////////////////////////////
/////////////////// HOUSE IMPLEMENTATION ///////////////////
/////////////////////////////////////////////////////////////////

class House : public Investment     // DERIVED CLASS
{
public:
    
    // Constructor & destructor
    House(string name, int price)
    : Investment(name, price)
    { }
    
    virtual ~House()
    {
        cout << "Destroying the house " << name() << endl;
    }
    
    // Specialized Functions
    virtual bool fungible() const
    {
        return false;
    }
    
    virtual string description() const
    {
        return "house";
    }
    
private:

};

///////////////////////////////////////////////////////////////

/*
void display(const Investment* inv)
{
    cout << inv->name();
    if (inv->fungible())
        cout << " (fungible)";
    cout << ": " << inv->description() << " bought for $" << inv->purchasePrice() << endl;
}

int main()
{
    Investment* portfolio[4];
    portfolio[0] = new Painting("Salvator Mundi", 450300000);
    // Stock holdings have a name, value, and ticker symbol
    portfolio[1] = new Stock("Alphabet", 100000, "GOOGL");
    portfolio[2] = new Stock("Symantec", 50000, "SYMC");
    portfolio[3] = new House("4 Privet Drive", 660000);
    
    for (int k = 0; k < 4; k++)
        display(portfolio[k]);
    
    // Clean up the investments before exiting
    cout << "Cleaning up" << endl;
    for (int k = 0; k < 4; k++)
        delete portfolio[k];
}

 OUTPUT
 Salvator Mundi: painting bought for $450300000
 Alphabet (fungible): stock trading as GOOGL bought for $100000
 Symantec (fungible): stock trading as SYMC bought for $50000
 4 Privet Drive: house bought for $660000
 Cleaning up
 Destroying Salvator Mundi, a painting
 Destroying Alphabet, a stock holding
 Destroying Symantec, a stock holding
 Destroying the house 4 Privet Drive
*/
