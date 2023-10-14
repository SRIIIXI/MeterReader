#ifndef DLMS_SCHEDULE_ENTRY_H
#define DLMS_SCHEDULE_ENTRY_H

#include "DLMSObject.h"

/**
* Executed scripts.
*/
class DLMSScheduleEntry
{
private:
    /**
    * Schedule entry index.
    */
    unsigned short m_Index;

    /**
     * Is Schedule entry enabled.
     */
    bool m_Enable;

    /**
     * Logical name of the Script table object.
     */
    std::string m_LogicalName;

    /**
     * Script identifier of the script to be executed.
     */
    unsigned short m_ScriptSelector;

    /**
    * Switch time.
    */
    Time m_SwitchTime;

    /**
     * Defines a period in minutes, in which an entry shall be processed after
     * power fail.
     */
    unsigned short m_ValidityWindow;

    /**
     * Days of the week on which the entry is valid.
     */
    DLMS_WEEKDAYS m_ExecWeekdays;

    /**
     * Perform the link to the IC “Special days table”, day_id.
     */
    std::string m_ExecSpecDays;

    /**
     * Date starting period in which the entry is valid.
     */
    Date m_BeginDate;

    /**
     * Date starting period in which the entry is valid.
     */
    Date m_EndDate;

public:
    /**
     * Get schedule entry index.
     *
     * Returns Entry index.
     */
    unsigned short GetIndex();

    /**
     * Set schedule entry index.
     *
     * value:
     *            Entry index.
     */
    void SetIndex(unsigned short value);

    /**
     * Is Schedule entry enabled.
     *
     * Returns True, if schedule entry is enabled.
     */
    bool GetEnable();

    /**
     * Enable schedule entry.
     *
     * value:
     *            Is Schedule entry enabled.
     */
    void SetEnable(bool value);

    /**
     * Returns logical name of the Script table object.
     *
     * Returns Logical name of the Script table object.
     */
    std::string GetLogicalName();

    void SetLogicalName(std::string value);

    /**
     * Get script identifier of the script to be executed.
     *
     * Returns Script identifier.
     */
    unsigned short GetScriptSelector();

    /**
     * Set script identifier of the script to be executed.
     *
     * value:
     *            Script identifier.
     */
    void SetScriptSelector(unsigned short value);

    /**
     * Returns Switch time.
     */
    Time& GetSwitchTime();

    /**
     * value: Switch time.
     */
    void SetSwitchTime(Time& value);

    /**
     * Defines a period in minutes, in which an entry shall be processed after
     * power fail.
     *
     * Returns Validity period in minutes.
     */
    unsigned short GetValidityWindow();

    /**
     * Defines a period in minutes, in which an entry shall be processed after
     * power fail.
     *
     * value:
     *            Validity period in minutes.
     */
    void SetValidityWindow(unsigned short value);

    /**
     * Get days of the week on which the entry is valid.
     *
     * Returns Bit array of valid week days.
     */
    DLMS_WEEKDAYS GetExecWeekdays();

    /**
     * Set days of the week on which the entry is valid.
     *
     * value:
     *            Bit array of valid week days.
     */
    void SetExecWeekdays(DLMS_WEEKDAYS value);

    /**
     * Perform the link to the IC Special days table.
     *
     * Returns day_id.
     */
    std::string& GetExecSpecDays();

    /**
     * Perform the link to the IC Special days table.
     *
     * value:
     *            day_id
     */
    void SetExecSpecDays(std::string& value);

    /**
     * Date starting period in which the entry is valid.
     *
     * Returns Begin date.
     */
    Date& GetBeginDate();

    /**
     * Date starting period in which the entry is valid.
     *
     * value:
     *            Begin date.
     */
    void SetBeginDate(Date& value);

    /**
     * Get date starting period in which the entry is valid.
     *
     * Returns End date.
     */
    Date& GetEndDate();

    /**
     * Set date starting period in which the entry is valid.
     *
     * value:
     *            End date.
     */
    void SetEndDate(Date& value);
};

#endif
