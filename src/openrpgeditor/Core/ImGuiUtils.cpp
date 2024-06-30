#include "Core/ImGuiUtils.hpp"
#include "imgui_internal.h"

namespace ImGui {
void BeginGroupPanel(const char* name, const ImVec2& size) {
  BeginGroup();

  auto itemSpacing = GetStyle().ItemSpacing;
  PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
  PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

  auto frameHeight = GetFrameHeight();
  BeginGroup();

  ImVec2 effectiveSize = size;
  if (size.x < 0.0f)
    effectiveSize.x = GetContentRegionAvail().x;
  else
    effectiveSize.x = size.x;
  Dummy(ImVec2(effectiveSize.x, 0.0f));

  Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
  SameLine(0.0f, 0.0f);
  BeginGroup();
  Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
  SameLine(0.0f, 0.0f);
  TextUnformatted(name);
  SameLine(0.0f, 0.0f);
  Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
  BeginGroup();

  PopStyleVar(2);

  GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
  GetCurrentWindow()->Size.x -= frameHeight;

  PushItemWidth(effectiveSize.x - frameHeight);
}

void EndGroupPanel() {
  PopItemWidth();

  auto itemSpacing = GetStyle().ItemSpacing;

  PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
  PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

  auto frameHeight = GetFrameHeight();

  // workaround for incorrect capture of columns/table width by placing
  // zero-sized dummy element in the same group, this ensure
  // max X cursor position is updated correctly
  SameLine(0.0f, 0.0f);
  Dummy(ImVec2(0.0f, 0.0f));

  EndGroup();

  // GetWindowDrawList()->AddRectFilled(GetItemRectMin(), GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

  EndGroup();

  SameLine(0.0f, 0.0f);
  Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
  Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

  EndGroup();

  auto itemMin = GetItemRectMin();
  auto itemMax = GetItemRectMax();
  // GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

  ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
  GetWindowDrawList()->AddRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f),
                               ImColor(GetStyleColorVec4(ImGuiCol_Border)), halfFrame.x);

  PopStyleVar(2);

  GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
  GetCurrentWindow()->Size.x += frameHeight;

  Dummy(ImVec2(0.0f, 0.0f));

  EndGroup();
}

bool LabelOverLineEdit(const char* id, const char* label, char* string, int len, float width, const char* tooltip) {
  BeginGroup();
  bool ret = false;
  {
    Text("%s", label);
    char* name = (char*)malloc(len);
    strncpy(name, string, len);
    SetNextItemWidth(width);
    if (InputText(id, name, len, ImGuiInputTextFlags_EnterReturnsTrue)) {
      strncpy(string, name, len);
      ret = true;
    }
    if (tooltip != nullptr) {
      SetItemTooltip("%s", tooltip);
    }
    free(name);
  }
  EndGroup();
  return ret;
}
/* Copy of ImGui::Selectable with borders enabled */
bool SelectableWithBorder(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg) {
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;

  // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
  ImGuiID id = window->GetID(label);
  ImVec2 label_size = CalcTextSize(label, NULL, true);
  ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
  ImVec2 pos = window->DC.CursorPos;
  pos.y += window->DC.CurrLineTextBaseOffset;
  ItemSize(size, 0.0f);

  // Fill horizontal space
  // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly
  // right-aligned sizes not visibly match other widgets.
  const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
  const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
  const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
  if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
    size.x = ImMax(label_size.x, max_x - min_x);

  // Text stays at the submission position, but bounding box may be extended on both sides
  const ImVec2 text_min = pos;
  const ImVec2 text_max(min_x + size.x, pos.y + size.y);

  // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing
  // between selectable.
  ImRect bb(min_x, pos.y, text_max.x, text_max.y);
  if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0) {
    const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
    const float spacing_y = style.ItemSpacing.y;
    const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
    const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
    bb.Min.x -= spacing_L;
    bb.Min.y -= spacing_U;
    bb.Max.x += (spacing_x - spacing_L);
    bb.Max.y += (spacing_y - spacing_U);
  }
  // if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

  // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every
  // Selectable..
  const float backup_clip_rect_min_x = window->ClipRect.Min.x;
  const float backup_clip_rect_max_x = window->ClipRect.Max.x;
  if (span_all_columns) {
    window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
    window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
  }

  const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
  const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
  if (span_all_columns) {
    window->ClipRect.Min.x = backup_clip_rect_min_x;
    window->ClipRect.Max.x = backup_clip_rect_max_x;
  }

  if (!item_add)
    return false;

  const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
  if (disabled_item && !disabled_global) // Only testing this as an optimization
    BeginDisabled();

  // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full
  // push on render only, which would be advantageous since most selectable are not selected.
  if (span_all_columns) {
    if (g.CurrentTable)
      TablePushBackgroundChannel();
    else if (window->DC.CurrentColumns)
      PushColumnsBackground();
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
    g.LastItemData.ClipRect = window->ClipRect;
  }

  // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
  ImGuiButtonFlags button_flags = 0;
  if (flags & ImGuiSelectableFlags_NoHoldingActiveID) {
    button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
  }
  if (flags & ImGuiSelectableFlags_NoSetKeyOwner) {
    button_flags |= ImGuiButtonFlags_NoSetKeyOwner;
  }
  if (flags & ImGuiSelectableFlags_SelectOnClick) {
    button_flags |= ImGuiButtonFlags_PressedOnClick;
  }
  if (flags & ImGuiSelectableFlags_SelectOnRelease) {
    button_flags |= ImGuiButtonFlags_PressedOnRelease;
  }
  if (flags & ImGuiSelectableFlags_AllowDoubleClick) {
    button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
  }
  if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) {
    button_flags |= ImGuiButtonFlags_AllowOverlap;
  }

  const bool was_selected = selected;
  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

  // Auto-select when moved into
  // - This will be more fully fleshed in the range-select branch
  // - This is not exposed as it won't nicely work with some user side handling of shift/control
  // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
  //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection()
  //   calling PushFocusScope())
  //   - (2) usage will fail with clipped items
  //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
  if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 &&
      g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
    if (g.NavJustMovedToId == id)
      selected = pressed = true;

  // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed
  // with gamepad/keyboard
  if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover))) {
    if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent) {
      SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId,
               WindowRectAbsToRel(window, bb)); // (bb == NavRect)
      g.NavDisableHighlight = true;
    }
  }
  if (pressed)
    MarkItemEdited(id);

  // In this branch, Selectable() cannot toggle the selection so this will never trigger.
  if (selected != was_selected) //-V547
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

  // Render
  if (hovered || selected) {
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive
                                  : hovered         ? ImGuiCol_HeaderHovered
                                                    : ImGuiCol_Header);
    RenderFrame(bb.Min, bb.Max, col, true, 0.0f);
  }
  if (g.NavId == id)
    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_Compact | ImGuiNavHighlightFlags_NoRounding);

  if (span_all_columns) {
    if (g.CurrentTable)
      TablePopBackgroundChannel();
    else if (window->DC.CurrentColumns)
      PopColumnsBackground();
  }

  RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);

  // Automatically close popups
  if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) &&
      !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
    CloseCurrentPopup();

  if (disabled_item && !disabled_global)
    EndDisabled();

  IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
  return pressed; //-V1020
}

void ActionTooltip(const char* action, const char* fmt, ...) {
  if (BeginTooltip()) {
    SeparatorText(action);
    va_list args;
    va_start(args, fmt);
    TextV(fmt, args);
    va_end(args);
    EndTooltip();
  }
}

} // namespace ImGui