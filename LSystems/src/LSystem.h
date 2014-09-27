#pragma once

#include <string>
#include <list>

using std::list;
using std::string;

//----------------------------------------------------------------------------------------------------------------------

class LSystemRule
{
public:
  LSystemRule (char from, string to)
  {
    mFrom = from;
    mTo   = to;
  }

public:
  char   mFrom;
  string mTo;
};

//----------------------------------------------------------------------------------------------------------------------

class LSystemRuleSet
{
public:
  LSystemRuleSet () 
  {
  }

  void addRule (LSystemRule *rule)
  {
    mRules.push_back (rule);
  }

  void applyRules (const string& before, string& after);

private:
  list<LSystemRule *> mRules;
};

//----------------------------------------------------------------------------------------------------------------------

class LSystemRenderer
{
public:
  virtual void draw (const string& lSystemString) = 0;
};

//----------------------------------------------------------------------------------------------------------------------

class LSystem
{
public:
  LSystem (const string& start, 
           LSystemRuleSet *const ruleSet, 
           LSystemRenderer *const renderer)
  : mStart (start), 
    mRuleSet (ruleSet),
    mRenderer (renderer),
    mCurrent (start)
  {
  }

  void update ();

  void update (unsigned int nofIterations);

  void reset ()
  {
    mCurrent = mStart;
  }

  void draw ()
  {
    mRenderer->draw (mCurrent);
  }

private:
  LSystemRuleSet  *const mRuleSet;
  LSystemRenderer *const mRenderer;

  const string mStart;
  string mCurrent;
};


//----------------------------------------------------------------------------------------------------------------------
