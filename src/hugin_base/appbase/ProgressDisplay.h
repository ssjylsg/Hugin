// -*- c-basic-offset: 4 -*-
/** @file ProgressDisplay.h
*
*  @author Ippei UKAI <ippei_ukai@mac.com>
*
*  $Id: $
*
*  !! based on ProgressDisplay in utils.h 1952 
*
*  This is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public
*  License as published by the Free Software Foundation; either
*  version 2 of the License, or (at your option) any later version.
*
*  This software is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public
*  License along with this software; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

namespace AppBase {

/**
 *
 */
class ProgressDisplay
{
    
// -- Task object --
    
protected:

    /**
     *
     */
    struct ProgressTask
    {
        ///
        ProgressTask() { };
        
        ///
        ProgressTask(const std::string& message,
                     const double& maxProgress,
                     const double& progressForParentTask, 
                     const bool& propagatesProgress)
            : message(message),
              maxProgress(maxProgress),
              progressForParentTask(progressForParentTask), 
              propagatesProgress(propagatesProgress),
              progress(0.0)
        { };
        
        ///
        std::string message;
        ///
        double progressForParentTask;
        ///
        double maxProgress;
        ///
        double progress;
        ///
        bool propagatesProgress;
        
        ///
        bool measuresProgress()
            { return maxProgress == 0; };
    };
    
    
// -- Const/Destructors --
    
protected:
    ///
    virtual ProgressDisplay()
        : o_newSubtaskProgress(0)
    {};
    
public:
    ///
    virtual ~ProgressDisplay();
    
    
// -- task interface --
    
protected:
    ///
    void startSubtaskWithTask(const ProgressTask& newSubtask)
    {
        subtasks.push_back(newSubtask);
        subtaskStarted();
        updateProgressDisplay();
    }
    
public:
    ///
    void setParentProgressOfNewSubtasks(double subtaskTotalProgress, bool propagatesProgress = false)
    {
        if(subtaskTotalProgress < 0)
            return;
        
        o_newSubtaskProgress = subtaskTotalProgress;
        o_newSubtaskPropagates = propagatesProgress;
    };
    
    ///
    void startSubtask(const std::string& message,
                      const double& maxProgress,
                      const double& progressForParentTask,
                      const bool& propagatesProgress = false)
    {
        ProgressTask newSubtask = ProgressTask(message, maxProgress, progressForParentTask, propagatesProgress);
        
        startSubtaskWithTask(newSubtask);
    };
    
    ///
    void startSubtask(const std::string& message,
                      const double& maxProgress = 0)
    {
        if(o_newSubtaskProgress > 0)
            startSubtask(message, maxProgress, o_newSubtaskProgress, o_newSubtaskPropagates);
        else
            startSubtask(message, maxProgress, 0, false);
    };
    
    ///
    void startSubtask(const double& maxProgress)
    { 
        startSubtask("", maxProgress);
    };
       
    ///
    virtual void setSubtaskMessage(const std::string& message)
        { getCurrentSubtask().message = message; };
    
    ///
    virtual std::string getSubtaskMessage() const
    { return getCurrentSubtask().message; };
    
    ///
    double getSubtaskMaxProgress() const
    {
        assert(!noSubtasksAvailable()); //[TODO] make it nicer:)
        return getCurrentSubtask().maxProgress;
    };
        
    ///
    double getSubtaskProgress() const
    {
        assert(!noSubtasksAvailable()); //[TODO] make it nicer:)
        return getCurrentSubtask().progress;
    };
    
    ///
    void updateSubtaskProgress(const double& newValue)
    {
        if(noSubtasksAvailable())
        {
            //[TODO] debug
            return;
        }
        
        if(getCurrentSubtask().progress > newValue)
        {
            //[TODO] debug
            return;
        }
        
        getCurrentSubtask().progress = max(newValue, getSubtaskMaxProgress());
        updateProgressDisplay();
    }
    
    ///
    void increaseSubtaskProgressBy(const double& deltaValue)
    {
        updateSubtaskProgress(getSubtaskProgress() + deltaValue);
    }
    
    ///
    void finishSubtask()
    {
        subtaskFinished();
        
        if (!tasks.back().measureProgress && tasks.size()>1) {
            tasks[tasks.size()-2].progress += tasks[tasks.size()-2].subStepProgress;
        }
        
        tasks.pop_back();
        updateProgressDisplay();
    };
    
    ///
    virtual bool wasCanceled()
        { return false; };
    
    
// -- callback interface --
    
protected:
    /** Template method, updates the display.
     *  You should override this method with your code of updating the display.
     *  The default implementation does nothing.
     */
    virtual void updateProgressDisplay() {};
    
    /** Template method, called when subtask is started.
     *  The default implementation does nothing.
     */
    virtual void subtaskStarted() {};
    
    /** Template method, called when subtask is finishing.
     *  The default implementation does nothing.
     */
    virtual void subtaskFinished() {};
         
    
// -- utility methods --
    
protected:
    ///
    void propagateProgress(const double& newProgress)
    {
        std::vector<ProgressTask>::reverse_iterator itr = tasks.rbegin();
        
        double diffFromPrev = newProgress - itr->progress;
        
        if(diffFromPrev <= 0)
            return;
        
        do {
            
            itr->progress += diffFromPrev;
            
            if(!itr->propagatesProgress)
                return;
            
            // scale previous change for higher level
            diffFromPrev *= itr->progressForParentTask / itr->maxProgress;
            
            itr++;
            
        } while(itr != tasks.rend());
    }

    ///
    ProgressTask& getCurrentSubtask() const
        { return o_subtasks.back(); };

    ///
    bool noSubtasksAvailable() const
        { return o_subtasks.empty(); };
    
    
// -- accessable variables --
    
protected:
    ///
    std::vector<ProgressTask> o_subtasks;
    
    ///
    double o_newSubtaskProgress;
    bool o_newSubtaskPropagates;
    
};


/** a progress display to print stuff to stdout (doesn't work properly on the
 *  windows console.
 */
//class StreamMultiProgressDisplay : public MultiProgressDisplay
//{
//public:
//    StreamMultiProgressDisplay(std::ostream & o, double minPrintStep=0.02)
//        : MultiProgressDisplay(minPrintStep),
//        m_stream(o), m_printedLines(0),
//        m_whizz("-\\|/"), m_whizzCount(0)
//    {
//            
//    }
//    
//    virtual ~StreamMultiProgressDisplay() {};
//    
//    /** update the display */
//    virtual void updateProgressDisplay()
//    {
//        int lines = m_printedLines;
//        // step back the line printed before.
//        if (lines !=0) {
//            m_stream << "\033[" << m_printedLines << "A"
//            << "\r";
//        }
//        m_printedLines = 0;
//        // build the message:
//        for (std::vector<ProgressTask>::iterator it = tasks.begin();
//             it != tasks.end(); ++it)
//        {
//            m_printedLines++;
//            char tmp[81];
//            tmp[80]=0;
//            if (it->measureProgress) {
//                snprintf(tmp,80,"%15s : %-50s : %3.0f %%",
//                         it->getShortMessage().c_str(),
//                         it->getMessage().c_str(),
//                         100 * it->getProgress());
//            } else if (! it->measureProgress && it+1 == tasks.end()) {
//                m_whizzCount = (++m_whizzCount) % (int)m_whizz.size();
//                snprintf(tmp,80,"%20s: %-50s :   %c ",
//                         it->getShortMessage().c_str(),
//                         it->getMessage().c_str(),
//                         m_whizz[m_whizzCount]);
//            } else {
//                snprintf(tmp,80,"%20s: %-50s :   - ",
//                         it->getShortMessage().c_str(),
//                         it->getMessage().c_str());
//            }
//            
//            m_stream << tmp << std::endl;
//        }
//        // print empty lines..
//        while (m_printedLines < lines) {
//            m_stream << "                                                                               " << std::endl;
//            m_printedLines++;
//        }
//    }
//    
//protected:
//    std::ostream & m_stream;
//    int m_printedLines;
//    std::string m_whizz;
//    int m_whizzCount;
//};



}; //namespace
    