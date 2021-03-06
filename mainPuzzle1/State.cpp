
#include "State.h"
#include "Blocks.h"
#include "Puzzle.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Example {

IMPLEMENT_SUBCLASS(State,"PuzzleState")

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(State)
    "blocks",      //  1: list of blocks
END_SLOTTABLE(State)

//------------------------------------------------------------------------------
//  Map slot table to handles 
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(State)
    ON_SLOT( 1, setSlotBlocks, Basic::PairStream )        
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
State::State()
{
   STANDARD_CONSTRUCTOR()

   for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
      blocks[i] = 0;
   }
   nblocks = 0;

   expanded = false;
   generation = 0;
}

State::State(const State& org, const Block* const nb, const unsigned int idx)
{
   STANDARD_CONSTRUCTOR()

   for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
      blocks[i] = 0;
   }
   nblocks = 0;

   setBlocks(org.blocks, org.nblocks);

   if (nb != 0 && idx < nblocks) {
      if (blocks[idx] != 0) blocks[idx]->unref();
      blocks[idx] = nb->clone();
   }
   sortBlocks();

   generation = org.getGeneration() + 1;
   expanded = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void State::copyData(const State& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
         blocks[i] = 0;
      }
      nblocks = 0;
   }

   expanded = org.expanded;
   generation = org.generation;

   setBlocks(org.blocks, org.nblocks);
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void State::deleteData()
{
   clearBlocks();
}

//------------------------------------------------------------------------------
// g()
//------------------------------------------------------------------------------
int State::g() const
{
  return getGeneration();
}

//------------------------------------------------------------------------------
// h()
//------------------------------------------------------------------------------
int State::h() const
{
#if 1
  int dx = blocks[0]->getX() - 2;
  if (dx < 0) dx = -dx;

  int dy = blocks[0]->getY() - 4;
  if (dy < 0) dy = -dy;

  int dd = 3 - dx - dy;
  if (dd < 0) dd = -dd;

  int gy = blocks[0]->getY() - 1;
  if (gy < 0) gy = -gy;
  return 100*dd + 200*gy;
#else
  return 0;
#endif
}

//------------------------------------------------------------------------------
// Returns the i'th block (range: 1 .. getNumberOfBlocks())
//------------------------------------------------------------------------------

// Returns the i'th block (range: 1 .. getNumberOfBlocks()) (const version)
const Block* State::getBlock(const unsigned int i) const
{
   const Block* p = 0;
   if (i > 0 && i <= nblocks) {
      p = blocks[i-1];
   }
   return p;
}

// Returns the block with ref ID
const Block* State::getBlockByRefNum(const unsigned int refId) const
{
   const Block* p = 0;
   for (unsigned int i = 0; i < nblocks && p == 0; i++) {
      if (refId == blocks[i]->getReferenceID()) {
         // found it
         p = blocks[i];
      }
   }
   return p;
}

//------------------------------------------------------------------------------
// Expand the state, s, and 
//  returns the state that matches the 'goal' state, if found, else zero
//------------------------------------------------------------------------------
const State* State::expand(const State* const goal, Puzzle* const puz)
{
   const State* endState = 0;
   expanded = true;

   if (goal != 0 && puz != 0) {

      // for each block contained in the state ...
      for (unsigned int idx = 0; idx < nblocks && endState == 0; idx++) {

         // Current block
         const Block* const cb = blocks[idx];

         // move right as far as we can, expanding new states as we go
         {
            bool collision = false;
            int dy = 0;
            for (int dx = 1; !collision && endState == 0 && cb->testMove(dx,dy); dx++) {
               Block* nb = cb->clone();
               nb->move(dx,dy);
               for (unsigned int j = 0; j < nblocks && !collision; j++) {
                  if ( j != idx ) {
                     collision = nb->collisionCheck(blocks[j]);
                  }
               }
               if ( !collision ) endState = stateFactory(nb, idx, goal, puz);
               nb->unref();
            }
         }

         // move left as far as we can, expanding new states as we go
         {
            bool collision = false;
            int dy = 0;
            for (int dx = -1; !collision && endState == 0 &&  cb->testMove(dx,dy); dx--) {
               Block* nb = cb->clone();
               nb->move(dx,dy);
               for (unsigned int j = 0; j < nblocks && !collision; j++) {
                  if ( j != idx ) {
                     collision = nb->collisionCheck(blocks[j]);
                  }
               }
               if ( !collision ) endState = stateFactory(nb, idx, goal, puz);
               nb->unref();
            }
         }

         // move up as far as we can, expanding new states as we go
         {
            bool collision = false;
            int dx = 0;
            for (int dy = 1; !collision && endState == 0 &&  cb->testMove(dx,dy); dy++) {
               Block* nb = cb->clone();
               nb->move(dx,dy);
               for (unsigned int j = 0; j < nblocks && !collision; j++) {
                  if ( j != idx ) {
                     collision = nb->collisionCheck(blocks[j]);
                  }
               }
               if ( !collision ) endState = stateFactory(nb, idx, goal, puz);
               nb->unref();
            }
         }

         // move down as far as we can, expanding new states as we go
         {
            bool collision = false;
            int dx = 0;
            for (int dy = -1; !collision && endState == 0 &&  cb->testMove(dx,dy); dy--) {
               Block* nb = cb->clone();
               nb->move(dx,dy);
               for (unsigned int j = 0; j < nblocks && !collision; j++) {
                  if ( j != idx ) {
                     collision = nb->collisionCheck(blocks[j]);
                  }
               }
               if ( !collision ) endState = stateFactory(nb, idx, goal, puz);
               nb->unref();
            }
         }
      }

   }

   return endState;
}

//------------------------------------------------------------------------------
// stateFactory -- create a new state (based on this one) and replace the block
// at index, idx, with the new block, nb.
//------------------------------------------------------------------------------
const State* State::stateFactory(const Block* const nb, const unsigned int idx, const State* const goal, Puzzle* const puz)
{
   const State* endState = 0;
   if (nb != 0 && idx < nblocks && goal != 0) {

      // create a new state with this block.
      State* ns = new State(*this,nb,idx);
      ns->container(this);

      // try to put the new state on the hash table
      if ( puz->putHash(ns) ) {
         // Ok .. this really IS a new state; check to see if we have
         // reached our goal state
         if (*ns == *goal) {
            // We found it!
            endState = ns;
         }
         else {
            // Not yet .. add it to the list of open (unexpanded) states
            puz->putOpen(ns);
         }
      }

      ns->unref();
   }

   return endState;
}

//------------------------------------------------------------------------------
// hash() -- Compute the state's hash index; 'rh' is rehash count; 'max' is the maximum index
//------------------------------------------------------------------------------
unsigned int State::hash(unsigned int rh, unsigned int max) const
{
   unsigned int h = 0;
   for (unsigned int idx = 0; idx < nblocks; idx++) {
      h += blocks[idx]->computeHashValue(idx,rh);
   }
   return h % max;
}

//------------------------------------------------------------------------------
// Comparison functions
//------------------------------------------------------------------------------

// operator== -- are two states equal
bool operator==(const State& ss1, const State& ss2)
{
   const State* s1 = &ss1;
   const State* s2 = &ss2;

   // If they have an unequal number of blocks, make sure s1 has the
   // lesser number of blocks
   if ( ss2.nblocks < ss1.nblocks ) {
      s1 = &ss2;
      s2 = &ss1;
   }

   // for all blocks in s1, find the block in s2 with the same id and
   // make sure it's in the same position.
   bool equal = true;
   for (unsigned int i = 0; i < s1->nblocks && equal; i++) {
      if ( *(s1->blocks[i]) != *(s2->blocks[i]) ) equal = false;
   }

   return equal;
}


// operator== -- are two states not equal
bool operator!=(const State& ss1, const State& ss2)
{
   return !(ss1 == ss2);
}

//------------------------------------------------------------------------------
// Support functions for the array of blocks
//------------------------------------------------------------------------------

// Sets from an array of pointers to the blocks
unsigned int State::setBlocks(const Block* const newBlocks[], const unsigned int numNumBlocks)
{
   // First clear out the old
   clearBlocks();
   
   // Copy the new
   if (newBlocks != 0 && numNumBlocks > 0) {
      unsigned int n = numNumBlocks;
      if (n > MAX_BLOCKS) n =MAX_BLOCKS;
      for (unsigned int i = 0; i < n; i++) {
         blocks[i] = newBlocks[i]->clone();
      }
      nblocks = n;
      sortBlocks();
   }
   return nblocks;
}

// sort the blocks
void State::sortBlocks()
{
   for (unsigned int i = 1; i < nblocks; i++) {
      for (int j = i; j > 0 && ( *blocks[j-1] > *blocks[j] ); j-- ) {
         const Block* bb = blocks[j];
         blocks[j] = blocks[j-1];
         blocks[j-1] = bb;
      }
   }
}

// Clear blocks
void State::clearBlocks()
{
   for (unsigned int i = 0; i < MAX_BLOCKS; i++) {
      if (blocks[i] != 0) blocks[i]->unref();
      blocks[i] = 0;
   }
   nblocks = 0;
}

//------------------------------------------------------------------------------
// Slot function(s)
//------------------------------------------------------------------------------

// Blocks (list of Blocks)
bool State::setSlotBlocks(const Basic::PairStream* const msg)
{
   bool ok = false;
   if (msg != 0) {

      // Array of blocks
      unsigned int n = 0;
      const Block* newBlocks[MAX_BLOCKS];

      // Assume all will be well
      ok = true;

      // Find all blocks (and check their type to make sure)
      const Basic::List::Item* item = msg->getFirstItem();
      while (item != 0 && n < MAX_BLOCKS && ok) {
         const Basic::Pair* pair = static_cast<const Basic::Pair*>(item->getValue());
         const Block* p = dynamic_cast<const Block*>( pair->object() );
         if (p != 0) {
            newBlocks[n++] = p;  // Save the point
         }
         else {
            std::cerr << "State::setSlotBlocks() ERROR -- non-block object was found in the list of blocks" << std::endl;
            ok = false;   // This one wasn't a block
         }
         item = item->getNext();
      }

      // If all is well and we have at least one block ---
      if (ok && n > 0) {
         setBlocks(newBlocks,n);
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() 
//------------------------------------------------------------------------------
Basic::Object* State::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& State::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   indent(sout, i+j);
   sout << "blocks: {" << std::endl;

   for (unsigned int ii = 0; ii < nblocks; ii++) {
      indent(sout, i+j+4);
      sout << (ii+1) << ": ";
      blocks[ii]->serialize(sout,(i+j+8));
   }

   indent(sout, i+j);
   sout << "}" << std::endl;

   BaseClass::serialize(sout,i+j,true);

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}


}  // End of Example namespace
}  // End of Eaagles namespace
