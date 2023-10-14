#include "AdjacentCell.h"

unsigned long AdjacentCell::GetCellId() {
    return m_CellId;
}

void AdjacentCell::SetCellId(unsigned long value) {
    m_CellId = value;
}

unsigned char AdjacentCell::GetSignalQuality() {
    return m_SignalQuality;
}

void AdjacentCell::SetSignalQuality(unsigned char value) {
    m_SignalQuality = value;
}
