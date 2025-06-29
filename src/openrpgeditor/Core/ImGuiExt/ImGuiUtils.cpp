#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_stacklayout.h"
#include "OREMath/Size.hpp"

#include <cstdio>
#include <format>

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
  GetWindowDrawList()->AddRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f), ImColor(GetStyleColorVec4(ImGuiCol_Border)), halfFrame.x);

  PopStyleVar(2);

  GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
  GetCurrentWindow()->Size.x += frameHeight;

  Dummy(ImVec2(0.0f, 0.0f));

  EndGroup();
}

bool LabelOverLineEdit(const char* id, const char* label, char* string, int len, float width, const char* tooltip, ImGuiInputTextFlags flags) {
  BeginGroup();
  bool ret = false;
  {
    Text("%s", label);
    char* name = (char*)malloc(len);
    strncpy(name, string, len);
    if (width != 0.f) {
      SetNextItemWidth(width);
    }
    if (InputText(id, name, len, flags)) {
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
  // FIXME: Not part of layout so not included in clipper calculation, but ItemSize currently doesn't allow offsetting
  // CursorPos.
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

  const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
  const ImGuiItemFlags extra_item_flags = disabled_item ? (ImGuiItemFlags)ImGuiItemFlags_Disabled : ImGuiItemFlags_None;
  bool is_visible;
  if (span_all_columns) {
    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every
    // Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
    window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    is_visible = ItemAdd(bb, id, NULL, extra_item_flags);
    window->ClipRect.Min.x = backup_clip_rect_min_x;
    window->ClipRect.Max.x = backup_clip_rect_max_x;
  } else {
    is_visible = ItemAdd(bb, id, NULL, extra_item_flags);
  }

  const bool is_multi_select = (g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect) != 0;
  if (!is_visible)
    if (!is_multi_select || !g.BoxSelectState.UnclipMode ||
        !g.BoxSelectState.UnclipRect.Overlaps(bb)) // Extra layer of "no logic clip" for box-select support (would be more overhead to add to ItemAdd)
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
  if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap)) {
    button_flags |= ImGuiButtonFlags_AllowOverlap;
  }

  // Multi-selection support (header)
  const bool was_selected = selected;
  if (is_multi_select) {
    // Handle multi-select + alter button flags for it
    MultiSelectItemHeader(id, &selected, &button_flags);
  }

  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

  // Multi-selection support (footer)
  if (is_multi_select) {
    MultiSelectItemFooter(id, &selected, &pressed);
  } else {
    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g.
    //   BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
      if (g.NavJustMovedToId == id)
        selected = pressed = true;
  }

  // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed
  // with keyboard/gamepad
  if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover))) {
    if (!g.NavHighlightItemUnderNav && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent) {
      SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
      if (g.IO.ConfigNavCursorVisibleAuto)
        g.NavCursorVisible = false;
    }
  }
  if (pressed)
    MarkItemEdited(id);

  if (selected != was_selected)
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

  // Render
  if (is_visible) {
    const bool highlighted = hovered || (flags & ImGuiSelectableFlags_Highlight);
    if (highlighted || selected) {
      // Between 1.91.0 and 1.91.4 we made selected Selectable use an arbitrary lerp between _Header and _HeaderHovered.
      // Removed that now. (#8106)
      ImU32 col = GetColorU32((held && highlighted) ? ImGuiCol_HeaderActive : highlighted ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
      RenderFrame(bb.Min, bb.Max, col, true, 0.0f);
    }
    if (g.NavId == id) {
      ImGuiNavRenderCursorFlags nav_render_cursor_flags = ImGuiNavRenderCursorFlags_Compact | ImGuiNavRenderCursorFlags_NoRounding;
      if (is_multi_select)
        nav_render_cursor_flags |= ImGuiNavRenderCursorFlags_AlwaysDraw; // Always show the nav rectangle
      RenderNavCursor(bb, id, nav_render_cursor_flags);
    }
  }

  if (span_all_columns) {
    if (g.CurrentTable)
      TablePopBackgroundChannel();
    else if (window->DC.CurrentColumns)
      PopColumnsBackground();
  }

  if (is_visible)
    RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);

  // Automatically close popups
  if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_NoAutoClosePopups) && (g.LastItemData.ItemFlags & ImGuiItemFlags_AutoClosePopups))
    CloseCurrentPopup();

  if (disabled_item && !disabled_global)
    EndDisabled();

  // Selectable() always returns a pressed state!
  // Users of BeginMultiSelect()/EndMultiSelect() scope: you may call ImGui::IsItemToggledSelection() to retrieve
  // selection toggle, only useful if you need that state updated (e.g. for rendering purpose) before reaching
  // EndMultiSelect().
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

int ButtonGroup(const char* id, const std::vector<std::string>& buttons, const bool isVertical, const std::vector<std::string>& tooltips) {
  int ret = -1;

  if (isVertical) {
    BeginVertical(id);
  } else {
    BeginHorizontal(id);
  }

  const auto& style = GetStyle();
  ImVec2 size;

  // First get the largest button size
  for (const auto& btn : buttons) {
    const auto sz = CalcTextSize(btn.c_str(), nullptr, true);
    if (sz.x > size.x || sz.y > size.y) {
      size = sz;
    }
  }

  // Make sure the button size can fit the text
  size += style.FramePadding * 2.f;

  for (int i = 0; i < buttons.size(); i++) {
    if (ButtonEx(std::format("{}##{}_button_{}", buttons[i], id, i).c_str(), size, ImGuiButtonFlags_AlignTextBaseLine)) {
      ret = i;
    }
    if (i < tooltips.size()) {
      SetItemTooltip("%s", tooltips[i].c_str());
    }
  }
  if (isVertical) {
    EndVertical();
  } else {
    EndHorizontal();
  }

  return ret;
}

ImVec2 GetDPIScaledSize(const ImVec2& size) { return size * GetCurrentContext()->FontScale * GetCurrentWindow()->FontWindowScale; }

float GetDPIScaledValue(const float value) { return value * GetCurrentContext()->FontScale * GetCurrentWindow()->FontWindowScale; }

static bool IsRootOfOpenMenuSet() {
  ImGuiContext& g = *GImGui;
  ImGuiWindow* window = g.CurrentWindow;
  if ((g.OpenPopupStack.Size <= g.BeginPopupStack.Size) || (window->Flags & ImGuiWindowFlags_ChildMenu))
    return false;

  // Initially we used 'upper_popup->OpenParentId == window->IDStack.back()' to differentiate multiple menu sets from each others
  // (e.g. inside menu bar vs loose menu items) based on parent ID.
  // This would however prevent the use of e.g. PushID() user code submitting menus.
  // Previously this worked between popup and a first child menu because the first child menu always had the _ChildWindow flag,
  // making hovering on parent popup possible while first child menu was focused - but this was generally a bug with other side effects.
  // Instead we don't treat Popup specifically (in order to consistently support menu features in them), maybe the first child menu of a Popup
  // doesn't have the _ChildWindow flag, and we rely on this IsRootOfOpenMenuSet() check to allow hovering between root window/popup and first child menu.
  // In the end, lack of ID check made it so we could no longer differentiate between separate menu sets. To compensate for that, we at least check parent window nav layer.
  // This fixes the most common case of menu opening on hover when moving between window content and menu bar. Multiple different menu sets in same nav layer would still
  // open on hover, but that should be a lesser problem, because if such menus are close in proximity in window content then it won't feel weird and if they are far apart
  // it likely won't be a problem anyone runs into.
  const ImGuiPopupData* upper_popup = &g.OpenPopupStack[g.BeginPopupStack.Size];
  if (window->DC.NavLayerCurrent != upper_popup->ParentNavLayer)
    return false;
  return upper_popup->Window && (upper_popup->Window->Flags & ImGuiWindowFlags_ChildMenu) && ImGui::IsWindowChildOf(upper_popup->Window, window, true, false);
}

bool MenuItemNoCheck(const char* label, const char* icon, const char* shortcut, bool selected, bool enabled) {
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  ImGuiStyle& style = g.Style;
  ImVec2 pos = window->DC.CursorPos;
  ImVec2 label_size = CalcTextSize(label, NULL, true);

  // See BeginMenuEx() for comments about this.
  const bool menuset_is_open = IsRootOfOpenMenuSet();
  if (menuset_is_open)
    PushItemFlag(ImGuiItemFlags_NoWindowHoverableCheck, true);

  // We've been using the equivalent of ImGuiSelectableFlags_SetNavIdOnHover on all Selectable() since early Nav system days (commit 43ee5d73),
  // but I am unsure whether this should be kept at all. For now moved it to be an opt-in feature used by menus only.
  bool pressed;
  PushID(label);
  if (!enabled)
    BeginDisabled();

  // We use ImGuiSelectableFlags_NoSetKeyOwner to allow down on one menu item, move, up on another.
  const ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner | ImGuiSelectableFlags_SetNavIdOnHover;
  const ImGuiMenuColumns* offsets = &window->DC.MenuColumns;
  if (window->DC.LayoutType == ImGuiLayoutType_Horizontal) {
    // Mimic the exact layout spacing of BeginMenu() to allow MenuItem() inside a menu bar, which is a little misleading but may be useful
    // Note that in this situation: we don't render the shortcut, we render a highlight instead of the selected tick mark.
    float w = label_size.x;
    window->DC.CursorPos.x += IM_TRUNC(style.ItemSpacing.x * 0.5f);
    ImVec2 text_pos(window->DC.CursorPos.x + offsets->OffsetLabel, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    PushStyleVarX(ImGuiStyleVar_ItemSpacing, style.ItemSpacing.x * 2.0f);
    pressed = Selectable("", selected, selectable_flags, ImVec2(w, 0.0f));
    PopStyleVar();
    if (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible)
      RenderText(text_pos, label);
    window->DC.CursorPos.x +=
        IM_TRUNC(style.ItemSpacing.x *
                 (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
  } else {
    // Menu item inside a vertical menu
    // (In a typical menu window where all items are BeginMenu() or MenuItem() calls, extra_w will always be 0.0f.
    //  Only when they are other items sticking out we're going to add spacing, yet only register minimum width into the layout system.
    float icon_w = (icon && icon[0]) ? CalcTextSize(icon, NULL).x : 0.0f;
    float shortcut_w = (shortcut && shortcut[0]) ? CalcTextSize(shortcut, NULL).x : 0.0f;
    float min_w = window->DC.MenuColumns.DeclColumns(icon_w, label_size.x, 0.f, 0.f); // Feedback for next frame
    float stretch_w = ImMax(0.0f, GetContentRegionAvail().x - min_w);
    window->DC.MenuColumns.OffsetMark = window->DC.MenuColumns.OffsetShortcut;
    pressed = Selectable("", false, selectable_flags | ImGuiSelectableFlags_SpanAvailWidth, ImVec2(min_w, label_size.y));
    if (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_Visible) {
      RenderText(pos + ImVec2(offsets->OffsetLabel, 0.0f), label);
      if (icon_w > 0.0f)
        RenderText(pos + ImVec2(offsets->OffsetIcon, 0.0f), icon);
      if (shortcut_w > 0.0f) {
        // PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);
        LogSetNextTextDecoration("(", ")");
        RenderText(pos + ImVec2(offsets->OffsetShortcut + stretch_w, 0.0f), shortcut, NULL, false);
        // PopStyleColor();
      }
    }
  }
  IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (selected ? ImGuiItemStatusFlags_Checked : 0));
  if (!enabled)
    EndDisabled();
  PopID();
  if (menuset_is_open)
    PopItemFlag();

  return pressed;
}

float GetMinimumPanelHeight() { return (GetFrameHeight() + ImGui::GetStyle().WindowPadding.y + ImGui::GetStyle().ItemSpacing.y); }

bool SizeableCheckbox(const char* label, bool* v, float height) {
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  const float square_sz = height > 0.f ? height : GetFrameHeight();
  const ImVec2 pos = window->DC.CursorPos;
  const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
  ItemSize(total_bb, style.FramePadding.y);
  const bool is_visible = ItemAdd(total_bb, id);
  const bool is_multi_select = (g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect) != 0;
  if (!is_visible)
    if (!is_multi_select || !g.BoxSelectState.UnclipMode || !g.BoxSelectState.UnclipRect.Overlaps(total_bb)) // Extra layer of "no logic clip" for box-select support
    {
      IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
      return false;
    }

  // Range-Selection/Multi-selection support (header)
  bool checked = *v;
  if (is_multi_select)
    MultiSelectItemHeader(id, &checked, NULL);

  bool hovered, held;
  bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

  // Range-Selection/Multi-selection support (footer)
  if (is_multi_select)
    MultiSelectItemFooter(id, &checked, &pressed);
  else if (pressed)
    checked = !checked;

  if (*v != checked) {
    *v = checked;
    pressed = true; // return value
    MarkItemEdited(id);
  }

  ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
  const float left = ImMax(square_sz / 2, GetFrameHeight() / 2);
  const float right = ImMin(square_sz / 2, GetFrameHeight() / 2);
  check_bb.Min.y += left - right;
  check_bb.Max.y += left - right;
  const bool mixed_value = (g.LastItemData.ItemFlags & ImGuiItemFlags_MixedValue) != 0;
  if (is_visible) {
    RenderNavCursor(total_bb, id);
    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
    ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
    if (mixed_value) {
      // Undocumented tristate/mixed/indeterminate checkbox (#2644)
      // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
      ImVec2 pad(ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)), ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)));
      window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
    } else if (*v) {
      const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 6.0f));
      RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    }
  }
  check_bb.Min.y -= left - right;
  check_bb.Max.y -= left - right;
  const ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
  if (g.LogEnabled)
    LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
  if (is_visible && label_size.x > 0.0f)
    RenderText(label_pos, label);

  IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
  return pressed;
}

bool BeginGroupBox(const char* label, const ImVec2& size, bool* selected, bool* clicked, const ImGuiChildFlags child_flags, const ImGuiWindowFlags window_flags) {
  ImVec2 realSize = size;
  char idString[4096];
  snprintf(idString, 4096, "%s_checkbox", label);
  auto id = GetID(label);
  auto labelId = GetID(idString);
  auto groupPos = GetCursorScreenPos();
  auto groupStart = groupPos;
  PushStyleVarY(ImGuiStyleVar_FramePadding, GetStyle().FramePadding.y * 0.5);
  groupPos.y += (GetFontSize() / 2) + GetStyle().ItemSpacing.y;
  SetCursorScreenPos(groupPos);
  groupPos.y += (GetFrameHeight() / 2);

  /* Pad the size to fit the header */
  bool fillRegion = false;
  if (size.x >= 0.f) {
    realSize.x += GetStyle().FramePadding.x + GetStyle().FrameBorderSize + GetStyle().WindowPadding.x;
  } else if (size.x <= -1.f) {
    realSize.x = (GetFontSize() / 2) + GetStyle().FrameBorderSize;
    fillRegion = true;
  }

  if (size.y >= 0.f) {
    realSize.y += (GetFontSize() / 2) + GetFrameHeight();
  } else if (size.y <= -1.f) {
    realSize.y = (GetFontSize() / 2) + GetFrameHeight();
    fillRegion = true;
  }
  PopStyleVar();

  if (clicked) {
    *clicked = false;
  }

  const auto visible = BeginChild(id, fillRegion ? size : realSize, child_flags | ImGuiChildFlags_FrameStyle, window_flags);
  if (visible) {}

  BeginDisabled(selected && !*selected);
  return visible;
}

void EndGroupBox() {
  EndDisabled();
  EndChild();
}

void ItemLabel(const char* title, const ImGuiItemLabelFlags flags) {
  ImGuiWindow* window = GetCurrentWindow();
  const ImVec2 lineStart = GetCursorScreenPos();
  const ImGuiStyle& style = GetStyle();
  float fullWidth = GetContentRegionAvail().x;
  float itemWidth = CalcItemWidth() + style.ItemSpacing.x;
  ImVec2 textSize = CalcTextSize(title);
  ImRect textRect;
  textRect.Min = ImGui::GetCursorScreenPos();
  if (flags & ImGuiItemLabelFlags_Right)
    textRect.Min.x = textRect.Min.x + itemWidth;
  textRect.Max = textRect.Min;
  textRect.Max.x += fullWidth - itemWidth;
  textRect.Max.y += textSize.y;

  SetCursorScreenPos(textRect.Min);

  AlignTextToFramePadding();
  // Adjust text rect manually because we render it directly into a drawlist instead of using public functions.
  textRect.Min.y += window->DC.CurrLineTextBaseOffset;
  textRect.Max.y += window->DC.CurrLineTextBaseOffset;

  ItemSize(textRect);
  if (ItemAdd(textRect, window->GetID(title))) {
    RenderTextEllipsis(ImGui::GetWindowDrawList(), textRect.Min, textRect.Max, textRect.Max.x, title, nullptr, &textSize);

    if (textRect.GetWidth() < textSize.x && ImGui::IsItemHovered())
      ImGui::SetTooltip("%s", title);
  }
  if (flags & ImGuiItemLabelFlags_Left) {
    SetCursorScreenPos(textRect.Max - ImVec2{0, textSize.y + window->DC.CurrLineTextBaseOffset});
    SameLine();
  } else if (flags & ImGuiItemLabelFlags_Right)
    SetCursorScreenPos(lineStart);
}

bool InputIntFormat(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags, const char* fmt) {
  // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
  const char* format = fmt != nullptr ? fmt : (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
  return InputScalar(label, ImGuiDataType_S32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
}

bool SpinScaler(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step, const void* p_step_fast, const char* format, ImGuiInputTextFlags flags) {
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  ImGuiStyle& style = g.Style;
  IM_ASSERT((flags & ImGuiInputTextFlags_EnterReturnsTrue) ==
            0); // Not supported by InputScalar(). Please open an issue if you this would be useful to you. Otherwise use IsItemDeactivatedAfterEdit()!

  if (format == NULL)
    format = DataTypeGetInfo(data_type)->PrintFmt;

  void* p_data_default = (g.NextItemData.HasFlags & ImGuiNextItemDataFlags_HasRefVal) ? &g.NextItemData.RefVal : &g.DataTypeZeroValue;

  char buf[64];
  if ((flags & ImGuiInputTextFlags_DisplayEmptyRefVal) && DataTypeCompare(data_type, p_data, p_data_default) == 0)
    buf[0] = 0;
  else
    DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, p_data, format);

  // Disable the MarkItemEdited() call in InputText but keep ImGuiItemStatusFlags_Edited.
  // We call MarkItemEdited() ourselves by comparing the actual data rather than the string.
  g.NextItemData.ItemFlags |= ImGuiItemFlags_NoMarkEdited;
  flags |= ImGuiInputTextFlags_AutoSelectAll | (ImGuiInputTextFlags)ImGuiInputTextFlags_LocalizeDecimalPoint;

  bool value_changed = false;
  if (p_step == NULL) {
    if (InputText(label, buf, IM_ARRAYSIZE(buf), flags))
      value_changed = DataTypeApplyFromText(buf, data_type, p_data, format, (flags & ImGuiInputTextFlags_ParseEmptyRefVal) ? p_data_default : NULL);
  } else {
    BeginGroup(); // The only purpose of the group here is to allow the caller to query item data e.g. IsItemActive()
    PushID(label);
    SetNextItemWidth(ImMax(1.0f, CalcItemWidth() - ((GetFrameHeight() / 2) + GetStyle().ItemSpacing.x)));
    if (InputText("", buf, IM_ARRAYSIZE(buf), flags)) // PushId(label) + "" gives us the expected ID from outside point of view
      value_changed = DataTypeApplyFromText(buf, data_type, p_data, format, (flags & ImGuiInputTextFlags_ParseEmptyRefVal) ? p_data_default : NULL);

    bool hovered = false; // IsItemHovered() || IsItemActive() || IsItemActiveAsInputText() || IsItemFocused();
    if (hovered && (g.IO.MouseWheel > 0.f || g.IO.MouseWheel < 0.f)) {
      if (!IsItemFocused()) {
        FocusItem();
      }
    }

    // Step buttons    // Step buttons
    const ImVec2 backup_frame_padding = style.FramePadding;
    style.FramePadding.x = style.FramePadding.y;
    if (flags & ImGuiInputTextFlags_ReadOnly) {
      hovered = false;
      BeginDisabled();
    }
    PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
    SameLine(0, style.ItemInnerSpacing.x);

    // start diffs
    float frame_height = GetFrameHeight();
    float arrow_size = std::floor(frame_height * .45f);
    float arrow_spacing = frame_height - 2.0f * arrow_size;

    BeginGroup();
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{g.Style.ItemSpacing.x, arrow_spacing});

    // save/change font size to draw arrow buttons correctly
    float org_font_size = GetDrawListSharedData()->FontSize;
    GetDrawListSharedData()->FontSize = arrow_size;

    if (ArrowButtonEx("+", ImGuiDir_Up, ImVec2(arrow_size, arrow_size)) || (GetIO().MouseWheel > 0 && hovered)) {
      DataTypeApplyOp(data_type, '+', p_data, p_data, g.IO.KeyCtrl && p_step_fast ? p_step_fast : p_step);
      value_changed = true;
    }

    if (ArrowButtonEx("-", ImGuiDir_Down, ImVec2(arrow_size, arrow_size)) || (GetIO().MouseWheel < 0 && hovered)) {
      DataTypeApplyOp(data_type, '-', p_data, p_data, g.IO.KeyCtrl && p_step_fast ? p_step_fast : p_step);
      value_changed = true;
    }

    PopItemFlag();
    if (flags & ImGuiInputTextFlags_ReadOnly)
      EndDisabled();

    // restore font size
    GetDrawListSharedData()->FontSize = org_font_size;

    PopStyleVar(1);
    EndGroup();
    // end diffs

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end) {
      SameLine(0, style.ItemInnerSpacing.x);
      TextEx(label, label_end);
    }

    style.FramePadding = backup_frame_padding;

    PopID();
    EndGroup();
  }

  g.LastItemData.ItemFlags &= ~ImGuiItemFlags_NoMarkEdited;
  if (value_changed)
    MarkItemEdited(g.LastItemData.ID);

  return value_changed;
}

bool SpinInt(const char* label, int* v, int step, int step_fast, const char* format, ImGuiInputTextFlags flags) {
  // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
  const char* fmt = format ? format : (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
  return SpinScaler(label, ImGuiDataType_S32, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), fmt, flags);
}

bool SpinFloat(const char* label, float* v, float step, float step_fast, const char* format, ImGuiInputTextFlags flags) {
  flags |= ImGuiInputTextFlags_CharsScientific;
  return SpinScaler(label, ImGuiDataType_Float, (void*)v, (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL), format, flags);
}

bool SpinDouble(const char* label, double* v, double step, double step_fast, const char* format, ImGuiInputTextFlags flags) {
  flags |= ImGuiInputTextFlags_CharsScientific;
  return SpinScaler(label, ImGuiDataType_Double, (void*)v, (void*)(step > 0.0 ? &step : NULL), (void*)(step_fast > 0.0 ? &step_fast : NULL), format, flags);
}

bool EllipsesButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags) {

  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  ImVec2 pos = window->DC.CursorPos;
  if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
      style.FramePadding.y <
          window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
    pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
  ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

  const ImRect bb(pos, pos + size);
  ItemSize(size, style.FramePadding.y);
  if (!ItemAdd(bb, id))
    return false;

  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

  // Render
  const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
  RenderNavCursor(bb, id);
  RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
  const char* ellipses = "\xE2\x80\xA6";
  const ImVec2 ellipses_size = CalcTextSize(ellipses);

  if (g.LogEnabled)
    LogSetNextTextDecoration("[", "]");
  ImRect bb_label = bb;
  bb_label.Max.x -= ellipses_size.x + style.FramePadding.x;
  RenderTextClipped(bb_label.Min + style.FramePadding, bb_label.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb_label);
  RenderTextClipped({bb.Max.x - (ellipses_size.x + style.FramePadding.x), bb.Min.y + style.FramePadding.y}, bb.Max - style.FramePadding, ellipses, NULL, &ellipses_size, style.ButtonTextAlign, &bb);

  // Automatically close popups
  // if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
  //    CloseCurrentPopup();

  IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
  return pressed;
}

bool EllipsesButton(const char* label, const ImVec2& size_arg) { return EllipsesButtonEx(label, size_arg, ImGuiButtonFlags_None); }

} // namespace ImGui