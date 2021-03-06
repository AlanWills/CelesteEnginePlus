#include "Debug/Profiling/Profiler.h"
#include "Time/Clock.h"
#include "Utils/StringUtils.h"

#include <chrono>


namespace Celeste
{
  //------------------------------------------------------------------------------------------------
  Profiler::Profiler(const std::string& profilingFileFullPath) :
    m_logger(profilingFileFullPath),
    m_profilingInfo(MAX_PROFILING_BLOCKS)
  {
    // We have a fixed max size so we can just reserve our map size straightaway to avoid runtime allocations
  }

  //------------------------------------------------------------------------------------------------
  Profiler::~Profiler()
  {
    /// Write all information to log here
    for (std::pair<const StringId, ProfilingBlock>& pair : m_profilingInfo)
    {
      // Close any open blocks
      if (!pair.second.isClosed())
      {
        endProfilingBlock(pair.second.getName());
      }

      // Log the block info to a file
      logProfilingBlockInfo(pair);
    }
    
    // When the logger is destroyed it will take care of flushing the front log buffer so we just need to log all the data
    // and it will take care of the rest
  }

  //------------------------------------------------------------------------------------------------
  void Profiler::logProfilingBlockInfo(const std::pair<StringId, ProfilingBlock>& profilingInfo)
  {
    // Write in chunks so we do not overflow the buffer

    std::string logString;
    logString.reserve(200);

    // Block name
    {
      logString.append("Profiling Block: ");
      logString.append(profilingInfo.second.getName());

      m_logger.log(logString, Log::Verbosity::kRaw, profilingInfo.second.getName().c_str(), "", 0);
      logString.clear();
    }

    // Num calls
    {
      logString.append("Number of calls: ");
      numericToStringAppend(profilingInfo.second.m_numCalls, logString);

      m_logger.log(logString, Log::Verbosity::kRaw, profilingInfo.second.getName().c_str(), "", 0);
      logString.clear();
    }

    // Average time
    {
      logString.append("Average time (s): ");
      numericToStringAppend(Clock::systemSeconds(profilingInfo.second.m_averageTimeTaken), logString);

      m_logger.log(logString, Log::Verbosity::kRaw, profilingInfo.second.getName().c_str(), "", 0);
      logString.clear();
    }

    // Last call time
    {
      logString.append("Last call time (s): ");
      numericToStringAppend(Clock::systemSeconds(profilingInfo.second.m_lastCallTimeTaken), logString);

      m_logger.log(logString, Log::Verbosity::kRaw, profilingInfo.second.getName().c_str(), "", 0);
      logString.clear();
    }
  }

  //------------------------------------------------------------------------------------------------
  void Profiler::beginProfilingBlock(const std::string& profilingBlockName)
  {
    m_currentBlockName = profilingBlockName;

    StringId blockStringId = internString(profilingBlockName);
    ProfilingBlock& profilingBlock = m_profilingInfo[blockStringId];
    profilingBlock.m_name = profilingBlockName;
    profilingBlock.m_startTime = std::chrono::system_clock::now();

    ASSERT(profilingBlock.isClosed());

    profilingBlock.open();

    ASSERT(!profilingBlock.isClosed());
  }

  //------------------------------------------------------------------------------------------------
  void Profiler::endProfilingBlock(const std::string& profilingBlockName)
  {
    StringId blockStringId = internString(profilingBlockName);

    ASSERT(m_profilingInfo.find(blockStringId) != m_profilingInfo.end());

    ProfilingBlock& profilingBlock = m_profilingInfo[blockStringId];

    std::chrono::seconds timeTaken = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - profilingBlock.m_startTime);
    std::chrono::seconds averageTimeTake = profilingBlock.m_averageTimeTaken;
    int numCalls = profilingBlock.m_numCalls;

    // Update profiling data
    profilingBlock.m_lastCallTimeTaken = timeTaken;
    profilingBlock.m_averageTimeTaken = (averageTimeTake * numCalls + timeTaken) / (numCalls + 1);
    profilingBlock.m_numCalls = numCalls + 1;

    ASSERT(!profilingBlock.isClosed());

    profilingBlock.close();

    ASSERT(profilingBlock.isClosed());
  }

  //------------------------------------------------------------------------------------------------
  const ProfilingBlock* Profiler::getProfilingBlock(const std::string& profilingBlockName)
  {
    StringId blockStringId = internString(profilingBlockName);

    if (m_profilingInfo.find(blockStringId) == m_profilingInfo.end())
    {
      ASSERT_FAIL_MSG("Profiling block does not exist");
      return nullptr;
    }

    return &m_profilingInfo.at(blockStringId);
  }
}