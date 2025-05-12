// PawnIOLib - Library and tooling source to be used with PawnIO.
// Copyright (C) 2023  namazso <admin@namazso.eu>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef PAWNIOLIB_LIBRARY_H
#define PAWNIOLIB_LIBRARY_H

#ifdef PawnIOLib_EXPORTS
#define PAWNIO_EXPORT __declspec(dllexport)
#else
#define PAWNIO_EXPORT __declspec(dllimport)
#endif

#define PAWNIOAPI EXTERN_C PAWNIO_EXPORT HRESULT STDAPICALLTYPE

/// Get PawnIOLib version.
///
/// @p version A pointer to a ULONG which receives the version.
/// @return A HRESULT.
PAWNIOAPI pawnio_version(PULONG version);

/// Open a PawnIO executor.
///
/// @p handle A handle to the executor, or NULL.
/// @return A HRESULT.
PAWNIOAPI pawnio_open(PHANDLE handle);

/// Load a PawnIO blob.
///
/// @p handle Handle from @c pawnio_open.
/// @p blob Blob to load.
/// @p size Size of blob.
/// @return A HRESULT.
PAWNIOAPI pawnio_load(HANDLE handle, const UCHAR* blob, SIZE_T size);

/// Executes a function from the loaded blob.
///
/// @p handle Handle from @c pawnio_open.
/// @p name Function name to execute.
/// @p in Input buffer.
/// @p in_size Input buffer count.
/// @p out Output buffer.
/// @p out_size Output buffer count.
/// @p return_size Entries written in out_size.
/// @return A HRESULT.
PAWNIOAPI pawnio_execute(
  HANDLE handle,
  PCSTR name,
  const ULONG64* in,
  SIZE_T in_size,
  PULONG64 out,
  SIZE_T out_size,
  PSIZE_T return_size
);

/// Close a PawnIO executor.
///
/// @p handle Handle from @c pawnio_open.
/// @return A HRESULT.
PAWNIOAPI pawnio_close(HANDLE handle);

#endif //PAWNIOLIB_LIBRARY_H
