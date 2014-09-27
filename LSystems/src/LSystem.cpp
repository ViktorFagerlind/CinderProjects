#include "LSystem.h"

//----------------------------------------------------------------------------------------------------------------------

void LSystemRuleSet::applyRules (const string& before, string& after)
{
  after.clear ();

  for (string::const_iterator beforeIt = before.begin (); beforeIt != before.end (); beforeIt++)
  {
    bool ruleApplied = false;
    char c = *beforeIt;

    // Apply rules
    for (list<LSystemRule *>::const_iterator rulesIt = mRules.begin (); rulesIt != mRules.end (); rulesIt++)
    {
      LSystemRule *rule = *rulesIt;

      if (c == rule->mFrom)
      {
        after.append (rule->mTo);
        ruleApplied = true;
        break;
      }
    }

    // Identity rule applied if none matches
    if (!ruleApplied)
      after.push_back (c);
  }
}

//----------------------------------------------------------------------------------------------------------------------

void LSystem::update ()
{
  string tmp;

  mRuleSet->applyRules (mCurrent, tmp);

  mCurrent = tmp;
}

//----------------------------------------------------------------------------------------------------------------------

void LSystem::update (unsigned int nofIterations)
{
  for (unsigned int i=0; i<nofIterations; i++)
  {
    update ();
  }
}
