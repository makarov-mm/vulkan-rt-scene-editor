# Vulkan Hardware Ray Tracing — Scene Editor

An interactive scene editor on top of a real-time, hardware-accelerated ray
tracer (`VK_KHR_ray_tracing_pipeline`). Add, delete, move and rotate objects in
a fully ray-traced scene — dispersive glass, soft area-light shadows and
progressive accumulation included — through a native icon toolbar.

Zero external dependencies beyond the Vulkan SDK itself: the window and the
toolbar are raw Win32, the icons load through GDI+ (part of Windows), and all
math is inline (no GLFW, no ImGui, no GLM).

## Toolbar

| Group | Buttons |
|-------|---------|
| Shapes | Sphere, diamond, cube, pyramid, cylinder, dodecahedron, supertoroid, supershape — click to add one to the scene |
| Tools  | **Select** (cursor), **Move**, **Rotate** — the active tool is highlighted |
| Danger | **Delete selected** (trash), **Delete all** (trash + ×) |

Icons are pre-rendered anti-aliased PNGs in `icons/`, loaded at runtime with
GDI+ (a system library that ships with Windows, so the zero-dependency rule
still holds). If a PNG is missing, the button falls back to a GDI-drawn icon.

## Mouse

| Input | Action |
|-------|--------|
| **RMB drag** | Orbit the camera (any tool) |
| Mouse wheel | Zoom |
| **LMB**, Select tool | Click an object to select it; click empty space to deselect; **Shift-click** toggles objects in and out of the selection; **drag** draws a marquee (rubber band) that selects everything it touches (Shift adds to the selection) |
| **LMB drag**, Move tool | Drag the selection in the ground plane; hold **Shift** to move vertically. Grabbing an unselected object selects it first |
| **LMB drag**, Rotate tool | Horizontal drag spins the selection about the world Y axis, vertical drag about the camera's right axis |
| Del | Delete the selection |
| Esc | Quit |

Selected objects get a warm rim highlight rendered by the ray tracer itself;
the marquee rectangle is also composited by the ray-gen shader over the final
image, so there is no GDI flicker over the swapchain.

## Rendering features

- **Instanced acceleration structures** — one BLAS per shape type, built once
  with `PREFER_FAST_TRACE`. Every scene object is a TLAS instance carrying its
  own 3×4 transform, so moving, rotating, adding or deleting an object only
  rewrites the instance buffer and rebuilds the TLAS — the triangle geometry
  is never touched.
- **Per-instance materials** — colour, material id, reflectivity and the
  selection flag live in an SSBO indexed with `gl_InstanceCustomIndexEXT` in
  the closest-hit shader; normals go from object to world space through
  `gl_ObjectToWorldEXT`.
- **Dispersive Fresnel glass** — per-channel IOR 1.50/1.52/1.54 with
  hero-wavelength sampling, total internal reflection, coloured glass tint.
- **Soft shadows** from a visible spherical area light (cone sampling with a
  distance-dependent penumbra).
- **Per-pixel adaptive temporal accumulation** — static pixels converge to a
  clean image; pixels whose primary hit moved reset, so dragged objects stay
  sharp. Any edit also restarts accumulation so changes always show up clean.
- Ray-gen / closest-hit / two miss shaders, a correctly aligned shader binding
  table, `maxPipelineRayRecursionDepth = 1` (all bounces iterate in ray-gen).

## Requirements

- Windows 10/11, x64
- A GPU with `VK_KHR_ray_tracing_pipeline` support (NVIDIA RTX, AMD RX 6000+,
  Intel Arc) and recent drivers
- [Vulkan SDK](https://vulkan.lunarg.com/) (the `VULKAN_SDK` environment
  variable must be set — the installer does this)
- Visual Studio 2022+ with the C++ workload

## Building

Open `VulkanRTSceneEditor.sln` and build (x64, Debug or Release). The
pre-build step compiles the GLSL shaders to SPIR-V with the SDK's
`glslangValidator`; the post-build step copies them next to the executable.

## Architecture notes

The editor path is deliberately cheap. On any scene edit:

1. The instance array (`VkAccelerationStructureInstanceKHR`, one per object)
   and the per-object shading SSBO are rewritten on the host.
2. The TLAS is rebuilt (`PREFER_FAST_BUILD`) — with at most 64 instances this
   is sub-millisecond.
3. The accumulation counter resets, and the per-pixel history test
   additionally resets exactly the pixels whose primary hit moved.

BLASes are static and never rebuilt, which is what lets them use
`PREFER_FAST_TRACE` for maximum traversal performance. Object picking is a
CPU ray against per-instance bounding spheres; marquee selection projects the
same bounding spheres to the screen and tests them against the rectangle.
