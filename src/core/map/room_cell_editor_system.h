#ifndef INCLUDED_MAP_ROOM_CELL_EDITOR_SYSTEM_H
#define INCLUDED_MAP_ROOM_CELL_EDITOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"
#include "room_editor_loaded_event.h"

namespace map {

class RoomCellEditorSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(RoomCellEditorSystem)
    RoomCellEditorSystem();
    static Opt<RoomCellEditorSystem> Get();
    void RemoveCells();
    void AddCells();
    EntranceType::Type GetEntranceType( glm::vec2 pos );
    Opt<Cell> GetCellFromScene( glm::vec2 pos );
    void SetCellsVisible( bool visible );
    bool IsCellsVisible() const;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
    void SwitchCellFilledState( glm::vec2 pos );
    void SwitchEntranceState( glm::vec2 pos, EntranceType::Type entrance );
private:
    Scene& mScene;
    AutoReg mOnEditorModeChanged;
    void OnEditorModeChanged(map::EditorModeChangedEvent const& Evt);
    void EnableSubsystems( bool enable );
    AutoReg mOnEditorBack;
    void OnEditorBack( map::EditorBackEvent const& Evt );
    Opt<RoomDesc> mRoomDesc;
    std::vector<int32_t> mCellGUIDs;
    int32_t mCellCount = 0;
    int32_t mCellSize = 0;
    ModelValue mCellEditorModel;
    ModelValue mCellCountPressModel;
    ModelValue mCellCountModel;
    ModelValue mCellSizePressModel;
    ModelValue mCellSizeModel;
    void CellCountPress( std::string modifier );
    void CellSizePress( std::string modifier );
    glm::vec2 mMousePos;
    AutoReg mMouseMoveId;
    bool mMouseLeftPressed = false;
    bool mMouseRightPressed = false;
    bool mCellsVisible = false;
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    enum MouseMode
    {
        Nothing,
        CellEntrance,
        CellFill
    } mMouseMode = Nothing;
    ModelValue mMouseModeModel;
    void SetMouseMode( std::string mode );
    AutoReg mOnRoomEditorLoaded;
    void OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_CELL_EDITOR_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
