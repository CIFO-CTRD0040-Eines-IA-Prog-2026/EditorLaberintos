#ifndef CONTROLSLEGEND_H
#define CONTROLSLEGEND_H

class ControlsLegend
{
public:
    ControlsLegend();

    void Render();
    void Toggle();

private:
    bool m_bVisible;
};

#endif
